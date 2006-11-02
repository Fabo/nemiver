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
 *GNU General Public License along with Goupil;
 *see the file COPYING.
 *If not, write to the Free Software Foundation,
 *Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *See COPYRIGHT file copyright information.
 */

#include <vector>
#include <iostream>
#include <glib/gi18n.h>
#include <libglademm.h>
#include <gtkmm/dialog.h>
#include <gtkmm/entry.h>
#include <gtkmm/filechooserbutton.h>
#include <gtkmm/stock.h>
#include "nmv-exception.h"
#include "nmv-run-program-dialog.h"
#include "nmv-env.h"
#include "nmv-ustring.h"
#include "nmv-ui-utils.h"

using namespace std ;
using namespace nemiver::common ;

namespace nemiver {

struct EnvVarModelColumns : public Gtk::TreeModel::ColumnRecord
{
    // I tried using UString here, but it didn't want to compile... jmj
    Gtk::TreeModelColumn<Glib::ustring> varname;
    Gtk::TreeModelColumn<Glib::ustring> value;
    EnvVarModelColumns() { add (varname); add (value); }
};

struct RunProgramDialog::Priv
{
    Gtk::TreeView* treeview_environment;
    Gtk::Button* remove_button;
    Gtk::Button* add_button;
    EnvVarModelColumns env_columns;
    Glib::RefPtr<Gtk::ListStore> model;
    Gtk::Dialog &dialog ;
    Glib::RefPtr<Gnome::Glade::Xml> glade ;


    Priv (Gtk::Dialog &a_dialog, const Glib::RefPtr<Gnome::Glade::Xml> &a_glade) :
        model(Gtk::ListStore::create(env_columns)),
        dialog (a_dialog),
        glade (a_glade)
    {
    }

    void init ()
    {
        treeview_environment =
            ui_utils::get_widget_from_glade<Gtk::TreeView>
                                                (glade, "treeview_environment");
        treeview_environment->set_model (model);
        treeview_environment->append_column_editable
                                        (_("Name"), env_columns.varname);
        treeview_environment->append_column_editable
                                            (_("Value"), env_columns.value);

        add_button =
            ui_utils::get_widget_from_glade<Gtk::Button>
                                                    (glade, "button_add_var");
        THROW_IF_FAIL (add_button) ;
        add_button->signal_clicked().connect(sigc::mem_fun(*this,
                    &RunProgramDialog::Priv::on_add_new_variable));

        remove_button = ui_utils::get_widget_from_glade<Gtk::Button>
                                                    (glade, "button_remove_var");
        THROW_IF_FAIL (remove_button) ;
        remove_button->signal_clicked().connect(sigc::mem_fun(*this,
                    &RunProgramDialog::Priv::on_remove_variable));

        // we need to disable / enable sensitivity of the "Remove variable" button
        // based on whether a variable is selected in the treeview.
        treeview_environment->get_selection ()->signal_changed ().connect
            (sigc::mem_fun
                 (*this,
                  &RunProgramDialog::Priv::on_variable_selection_changed));

        // activate the default action (execute) when pressing enter in the
        // arguments text box
        ui_utils::get_widget_from_glade<Gtk::Entry>
                            (glade, "argumentsentry")->set_activates_default () ;
    }

    void on_add_new_variable ()
    {
        THROW_IF_FAIL(model);
        THROW_IF_FAIL(treeview_environment);
        Gtk::TreeModel::iterator treeiter = model->append ();
        Gtk::TreeModel::Path path = model->get_path (treeiter);
        // activate the first cell of the newly added row so that the user can start
        // typing in the name and value of the variable
        treeview_environment->set_cursor (path,
                *treeview_environment->get_column (0), true);
    }

    void on_remove_variable ()
    {
        THROW_IF_FAIL(treeview_environment);
        Gtk::TreeModel::iterator treeiter =
            treeview_environment->get_selection ()->get_selected ();
        if (treeiter)
        {
            model->erase(treeiter);
        }
    }

    void on_variable_selection_changed ()
    {
        THROW_IF_FAIL (remove_button) ;
        if (treeview_environment->get_selection ()->count_selected_rows ())
        {
            remove_button->set_sensitive();
        }
        else
        {
            remove_button->set_sensitive(false);
        }
    }

    void on_activate_textentry(void)
    {
        dialog.activate_default ();
    }
};

RunProgramDialog::RunProgramDialog (const UString &a_root_path) :
    Dialog (a_root_path, "runprogramdialog.glade", "runprogramdialog")
{
    m_priv = new Priv (widget (), glade ());
    THROW_IF_FAIL (m_priv);

    working_directory (Glib::get_current_dir ()) ;
}

RunProgramDialog::~RunProgramDialog ()
{
}

UString
RunProgramDialog::program_name () const
{
    Gtk::FileChooserButton *chooser =
        ui_utils::get_widget_from_glade<Gtk::FileChooserButton>
                                        (glade (), "filechooserbutton_program") ;
    return chooser->get_filename () ;
}

void
RunProgramDialog::program_name (const UString &a_name)
{
    Gtk::FileChooserButton *chooser =
        ui_utils::get_widget_from_glade<Gtk::FileChooserButton>
                                    (glade (), "filechooserbutton_program") ;
    chooser->set_filename (a_name) ;
}

UString
RunProgramDialog::arguments () const
{
    Gtk::Entry *entry =
        ui_utils::get_widget_from_glade<Gtk::Entry> (glade (), "argumentsentry");
    return entry->get_text () ;
}

void
RunProgramDialog::arguments (const UString &a_args)
{
    Gtk::Entry *entry =
        ui_utils::get_widget_from_glade<Gtk::Entry> (glade (), "argumentsentry");
    entry->set_text (a_args) ;
}

UString
RunProgramDialog::working_directory () const
{
    Gtk::FileChooserButton *chooser =
        ui_utils::get_widget_from_glade<Gtk::FileChooserButton>
                                        (glade (), "filechooserbutton_workingdir");
    return chooser->get_filename () ;
}

void
RunProgramDialog::working_directory (const UString &a_dir)
{
    Gtk::FileChooserButton *chooser =
        ui_utils::get_widget_from_glade<Gtk::FileChooserButton>
            (glade (), "filechooserbutton_workingdir");
    if (a_dir == "" || a_dir == ".") {
        chooser->set_filename (Glib::locale_to_utf8 (Glib::get_current_dir ()));
    } else {
        chooser->set_filename (a_dir) ;
    }
}

map<UString, UString>
RunProgramDialog::environment_variables () const
{
    THROW_IF_FAIL (m_priv);
    THROW_IF_FAIL (m_priv->model) ;
    map<UString, UString> env_vars;
    for (Gtk::TreeModel::iterator iter = m_priv->model->children().begin ();
            iter != m_priv->model->children().end(); ++iter) {
        // for some reason I have to explicitly convert from Glib::ustring to
        // UString here or it won't compile
        env_vars[UString((*iter)[m_priv->env_columns.varname])] =
            UString((*iter)[m_priv->env_columns.value]);
    }
    return env_vars;
}

void
RunProgramDialog::environment_variables (const map<UString, UString> &vars)
{
    THROW_IF_FAIL (m_priv) ;
    THROW_IF_FAIL (m_priv->model) ;
    // clear out the old data so we can set the new data
    m_priv->model->clear();
    for (map<UString, UString>::const_iterator iter = vars.begin();
         iter != vars.end();
         ++iter) {
        Gtk::TreeModel::iterator treeiter = m_priv->model->append();
        (*treeiter)[m_priv->env_columns.varname] = iter->first;
        (*treeiter)[m_priv->env_columns.value] = iter->second;
    }
}

}//end namespace nemiver

