//Author: Dodji Seketeli
/*
 *This file is part of the Nemiver project
 *
 *Nemiver is free software; you can redistribute
 *it and/or modify it under the terms of
 *the GNU General Public License as published by the
 *Free Software Foundation; either version 2,
 *or (at your option) any later version.
 *
 *Nemiver is distributed in the hope that it will
 *be useful, but WITHOUT ANY WARRANTY;
 *without even the implied warranty of
 *MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *See the GNU General Public License for more details.
 *
 *You should have received a copy of the
 *GNU General Public License along with Nemiver;
 *see the file COPYING.
 *If not, write to the Free Software Foundation,
 *Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *See COPYRIGHT file copyright information.
 */
#include "config.h"
#include "nmv-terminal.h"
#if !(defined(__FreeBSD__) || defined(__OpenBSD__))
# include <pty.h>
#else
# include <sys/types.h>
# include <sys/ioctl.h>
# include <termios.h>
#if defined(__FreeBSD__)
# include <libutil.h>
#else
# include <util.h>
#endif
#endif
#include <unistd.h>
#include <iostream>
#include <tr1/tuple>
#include <gtkmm/bin.h>
#include <gtkmm/main.h>
#include <gtkmm/window.h>
#include <gtkmm/adjustment.h>
#include <gtkmm/menu.h>
#include <gtkmm/builder.h>
#include <gtkmm/uimanager.h>
#include <vte/vte.h>
#include <glib/gi18n.h>
#include "common/nmv-exception.h"
#include "common/nmv-log-stream-utils.h"
#include "common/nmv-env.h"
#include "nmv-ui-utils.h"

NEMIVER_BEGIN_NAMESPACE(nemiver)

using namespace common;

typedef std::tr1::tuple<VteTerminal*&,
                   Gtk::Menu*&,
                   Glib::RefPtr<Gtk::ActionGroup>&> TerminalPrivDataTuple;

static bool
on_button_press_signal (GtkWidget*,
                        GdkEventButton *a_event,
                        TerminalPrivDataTuple *a_tuple)
{
    if (a_event->type != GDK_BUTTON_PRESS || a_event->button != 3) {
        return false;
    }

    NEMIVER_TRY

    THROW_IF_FAIL (a_tuple);
    VteTerminal*& vte = std::tr1::get<0> (*a_tuple);
    Gtk::Menu*& menu = std::tr1::get<1> (*a_tuple);
    Glib::RefPtr<Gtk::ActionGroup>& action_group = std::tr1::get<2> (*a_tuple);

    THROW_IF_FAIL (vte);
    THROW_IF_FAIL (action_group);
    THROW_IF_FAIL (a_event);

    Glib::RefPtr<Gtk::Clipboard> clipboard = Gtk::Clipboard::get();
    if (clipboard) {
         action_group->get_action ("PasteAction")->set_sensitive
                (clipboard->wait_is_text_available ());
    }

    action_group->get_action ("CopyAction")->set_sensitive
            (vte_terminal_get_has_selection (vte));
    menu->popup (a_event->button, a_event->time);

    NEMIVER_CATCH
    return true;
}

struct Terminal::Priv {
    //the master pty of the terminal (and of the whole process)
    int master_pty;
    int slave_pty;
    //the real vte terminal widget
    VteTerminal *vte;
    //the same object as
    //m_vte, but wrapped as a Gtk::Widget
    Gtk::Widget *widget;
    Glib::RefPtr<Gtk::Adjustment> adjustment;
    Gtk::Menu *menu;
    Glib::RefPtr<Gtk::ActionGroup> action_group;

    // Point to vte, menu, and action_group variables
    // Used by the on_button_press_signal event to show contextual menu
    TerminalPrivDataTuple popup_user_data;

    Priv (const string &a_menu_file_path,
          const Glib::RefPtr<Gtk::UIManager> &a_ui_manager) :
        master_pty (0),
        slave_pty (0),
        vte (0),
        widget (0),
        adjustment (0),
        menu (0),
        popup_user_data (vte, menu, action_group)
    {
        init_actions ();
        init_body (a_menu_file_path, a_ui_manager);
    }

    void init_body (const string &a_menu_file_path,
                    const Glib::RefPtr<Gtk::UIManager> &a_ui_manager)
    {
        GtkWidget *w = vte_terminal_new ();
        vte = VTE_TERMINAL (w);
        THROW_IF_FAIL (vte);

        // Mandatory for vte 0.14	
        vte_terminal_set_font_from_string (vte, "monospace");

        vte_terminal_set_scroll_on_output (vte, TRUE);
        vte_terminal_set_scrollback_lines (vte, 1000);
        vte_terminal_set_emulation (vte, "xterm");

        widget = Glib::wrap (w);
        THROW_IF_FAIL (widget);
        widget->set_manage ();
        widget->reference ();

        adjustment = Glib::wrap (vte_terminal_get_adjustment (vte));
        THROW_IF_FAIL (adjustment);
        adjustment->reference ();

        THROW_IF_FAIL (init_pty ());

        THROW_IF_FAIL (a_ui_manager);
        a_ui_manager->add_ui_from_file (a_menu_file_path);
        a_ui_manager->insert_action_group (action_group);
        menu = dynamic_cast<Gtk::Menu*>
                (a_ui_manager->get_widget ("/TerminalMenu"));
        THROW_IF_FAIL (menu);

        g_signal_connect (vte,
                          "button-press-event",
                          G_CALLBACK (on_button_press_signal),
                          &popup_user_data);
    }

    void init_actions ()
    {
        action_group = Gtk::ActionGroup::create ();
        action_group->add (Gtk::Action::create
                ("CopyAction",
                 Gtk::Stock::COPY,
                 "_Copy",
                 _("Copy the selection")),
                 sigc::mem_fun (*this,
                                &Terminal::Priv::on_copy_signal));
        action_group->add (Gtk::Action::create
                ("PasteAction",
                 Gtk::Stock::PASTE,
                 "_Paste",
                 _("Paste the clipboard")),
                 sigc::mem_fun (*this,
                                &Terminal::Priv::on_paste_signal));
        action_group->add (Gtk::Action::create
                ("ResetAction",
                 Gtk::StockID (""),
                 "_Reset",
                 _("Reset the terminal")),
                 sigc::mem_fun (*this,
                                &Terminal::Priv::on_reset_signal));
    }

    void on_reset_signal ()
    {
        NEMIVER_TRY

        reset ();

        NEMIVER_CATCH
    }

    void on_copy_signal ()
    {
        NEMIVER_TRY

        copy ();

        NEMIVER_CATCH
    }

    void on_paste_signal ()
    {
        NEMIVER_TRY

        paste ();

        NEMIVER_CATCH
    }

    void reset ()
    {
        THROW_IF_FAIL (vte);
        vte_terminal_reset (vte, true, true);
    }

    void copy ()
    {
        THROW_IF_FAIL (vte);
        vte_terminal_copy_clipboard (vte);
    }

    void paste ()
    {
        THROW_IF_FAIL (vte);
        vte_terminal_paste_clipboard (vte);
    }

    ~Priv ()
    {
        if (slave_pty) {
            close (slave_pty);
            slave_pty = 0;
        }

        if (master_pty) {
            close (master_pty);
            master_pty = 0;
        }

        if (widget) {
            widget->unreference ();
            widget = 0;
            vte = 0;
        }
    }

    bool init_pty ()
    {
        if (openpty (&master_pty, &slave_pty, 0, 0, 0)) {
            LOG_ERROR ("oops");
            return false;
        }
        THROW_IF_FAIL (slave_pty);
        THROW_IF_FAIL (master_pty);

        if (grantpt (master_pty)) {
            LOG_ERROR ("oops");
            return false;
        }

        if (unlockpt (master_pty)) {
            LOG_ERROR ("oops");
            return false;
        }

        vte_terminal_set_pty (vte, master_pty);
        return true;
    }
};//end Terminal::Priv

Terminal::Terminal (const string &a_menu_file_path,
                    const Glib::RefPtr<Gtk::UIManager> &a_ui_manager)
{
    m_priv.reset (new Priv (a_menu_file_path, a_ui_manager));
}

Terminal::~Terminal ()
{
    LOG_D ("deleted, ", "destructor-domain");
}


Gtk::Widget&
Terminal::widget () const
{
    THROW_IF_FAIL (m_priv->widget && m_priv->vte);
    return *m_priv->widget;
}

Glib::RefPtr<Gtk::Adjustment>
Terminal::adjustment () const
{
    THROW_IF_FAIL (m_priv);
    THROW_IF_FAIL (m_priv->adjustment);
    return m_priv->adjustment;
}

UString
Terminal::slave_pts_name () const
{
    THROW_IF_FAIL (m_priv);
    UString result;

    if (!m_priv->master_pty) {
        LOG_ERROR ("oops");
        return result;
    }

    result = ptsname (m_priv->master_pty);
    return result;
}

void
Terminal::modify_font (const Pango::FontDescription &font_desc)
{
    THROW_IF_FAIL (m_priv);
    vte_terminal_set_font (m_priv->vte, font_desc.gobj());
}

void
Terminal::feed (const UString &a_text)
{
    THROW_IF_FAIL (m_priv);
    if (!a_text.empty ())
        vte_terminal_feed (m_priv->vte, a_text.c_str (), a_text.size ());
}

int
Terminal::slave_fd () const
{
    THROW_IF_FAIL (m_priv);
    return m_priv->slave_pty;
}

NEMIVER_END_NAMESPACE(nemiver)

