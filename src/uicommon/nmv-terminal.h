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

#ifndef NMV_TERMINAL
#define NMV_TERMINAL

#include "common/nmv-safe-ptr-utils.h"
#include "common/nmv-env.h"
#include "common/nmv-ustring.h"
#include "common/nmv-object.h"
#include <string>
#include <glibmm/refptr.h>

using nemiver::common::UString;
using nemiver::common::Object;
using nemiver::common::SafePtr;

using std::string;

namespace Gtk {
    class Widget;
    class Adjustment;
    class UIManager;
}

namespace Pango {
    class FontDescription;
}

NEMIVER_BEGIN_NAMESPACE(nemiver)

class Terminal : public Object {
    //non copyable
    Terminal (const Terminal &);
    Terminal& operator= (const Terminal &);

    struct Priv;
    SafePtr<Priv> m_priv;

public:

    Terminal (const string &a_menu_file_path,
              const Glib::RefPtr<Gtk::UIManager> &a_ui_manager);
    ~Terminal ();
    Gtk::Widget& widget () const;
    Glib::RefPtr<Gtk::Adjustment> adjustment () const;
    UString slave_pts_name () const;
    int slave_fd () const;
    void modify_font (const Pango::FontDescription &font_desc);
    void feed (const UString &a_text);
};//end class Terminal

NEMIVER_END_NAMESPACE(nemiver)
#endif //NMV_TERMINAL
