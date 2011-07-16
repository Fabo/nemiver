// Author: Dodji Seketeli
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
#ifndef __NMV_SOURCE_EDITOR_H__
#define __NMV_SOURCE_EDITOR_H__

#include <list>
#include <functional>
#include <map>
#include <gtkmm/box.h>
#include <gtksourceviewmm/view.h>
#include "common/nmv-safe-ptr-utils.h"
#include "common/nmv-ustring.h"
#include "common/nmv-address.h"
#include "common/nmv-range.h"
#include "common/nmv-asm-instr.h"
#include "common/nmv-loc.h"

using Gsv::View;
using Gsv::Buffer;
using Gtk::VBox;
using nemiver::common::SafePtr;
using nemiver::common::UString;
using nemiver::common::Address;
using nemiver::common::Loc;
using std::list;
using std::map;

NEMIVER_BEGIN_NAMESPACE (nemiver);

extern const char* BREAKPOINT_ENABLED_CATEGORY;
extern const char* BREAKPOINT_DISABLED_CATEGORY;
extern const char* COUNTPOINT_CATEGORY;
extern const char* WHERE_CATEGORY;

extern const char* WHERE_MARK;


class SourceEditor : public  VBox {
    //non copyable
    SourceEditor (const SourceEditor&);
    SourceEditor& operator= (const SourceEditor&);
    struct Priv;
    SafePtr<Priv> m_priv;

    void init ();
    SourceEditor ();

public:

    enum BufferType {
        BUFFER_TYPE_UNDEFINED,
        BUFFER_TYPE_SOURCE,
        BUFFER_TYPE_ASSEMBLY
    };


    explicit SourceEditor (const UString &a_root_dir,
                           Glib::RefPtr<Buffer> &a_buf,
                           bool a_composite = false);
    virtual ~SourceEditor ();
    View& source_view () const;
    int current_line () const;
    void current_line (int a_line);
    int current_column () const;
    const Loc* current_location () const;
    void current_column (int &a_col);
    bool move_where_marker_to_line (int a_line, bool a_do_scroll = true);
    void unset_where_marker ();
    bool set_visual_breakpoint_at_line (int a_line,
					bool a_is_count_point,
					bool a_enabled);
    bool remove_visual_breakpoint_from_line (int a_line);
    void clear_decorations ();
    bool is_visual_breakpoint_set_at_line (int a_line) const;
    bool scroll_to_line (int a_line);
    void scroll_to_iter (Gtk::TextIter &a_iter);
    void set_path (const UString &a_path);
    void get_path (UString &a_path) const;
    const UString& get_path () const;
    void get_file_name (UString &a_file_name);
    bool get_word_at_position (int a_x,
                               int a_y,
                               UString &a_word,
                               Gdk::Rectangle &a_start_rect,
                               Gdk::Rectangle &a_end_rect) const;

    bool do_search (const UString &a_str,
                    Gtk::TextIter &a_start,
                    Gtk::TextIter &a_end,
                    bool a_match_case=false,
                    bool a_match_entire_word=false,
                    bool a_search_backwards=false,
                    bool a_clear_selection=false);

    static bool get_file_mime_type (const UString &a_path,
				    UString &a_mime_type);

    static bool setup_buffer_mime_and_lang (Glib::RefPtr<Buffer> &a_buf,
                                            const std::string &a_mime_type =
                                                "text/x-c++",
                                            const std::string &a_path = "");

    static Glib::RefPtr<Buffer> create_source_buffer ();

    static bool load_file (const UString &a_path,
			   const std::list<std::string> &a_supported_encodings,
			   bool a_enable_syntaxt_highlight,
			   Glib::RefPtr<Buffer> &a_source_buffer);

    /// \name Assembly source buffer handling.
    /// @{

    /// A assembly buffer is a buffer which content doesn't come
    /// directly from a file. It as been composed in memory. We use
    /// assembly buffers to represent the assembly view of a text file
    /// being debugged.
    /// Unlike non-assembly buffers, meaningful locations inside the buffer
    /// are not necessarily line numbers.
    /// In an assembly view, a meaningful location is the address
    /// of a machine instruction. So there somehow must be a kind of mapping
    /// between the location used for the assembly buffer and the actual
    /// line number, because the underlying Buffer implementation
    /// relies on line numbers anyhow.

    BufferType get_buffer_type () const;

    bool current_address (Address&) const;

    void register_assembly_source_buffer
                        (Glib::RefPtr<Buffer> &a_buf);

    void register_non_assembly_source_buffer
                                    (Glib::RefPtr<Buffer> &a_buf);

    Glib::RefPtr<Buffer> get_assembly_source_buffer () const;

    Glib::RefPtr<Buffer> get_non_assembly_source_buffer () const;

    bool switch_to_assembly_source_buffer ();

    bool switch_to_non_assembly_source_buffer ();

    bool assembly_buf_addr_to_line (const Address&, bool, int&) const;
    bool assembly_buf_line_to_addr (int, Address &) const;
    bool get_assembly_address_range (common::Range &a) const;
    bool move_where_marker_to_address (const Address &address,
                                       bool do_scroll,
				       bool a_approximate);
    bool place_cursor_at_line (size_t);
    bool place_cursor_at_address (const Address &);
    bool set_visual_breakpoint_at_address (const Address &a_address,
					   bool is_countpoint,
                                           bool enabled);
    bool remove_visual_breakpoint_from_address (const Address &);
    bool scroll_to_address (const Address &a_address,
			    bool a_approximate);

    static bool add_asm (const common::DisassembleInfo &/*a_info*/,
			 const std::list<common::Asm> &a_asm,
			 bool a_append,
			 const list<UString> &a_src_search_dirs,
			 list<UString> &a_session_dirs,
			 std::map<UString, bool> &a_ignore_paths,
			 Glib::RefPtr<Buffer> &a_buf);

    static bool load_asm (const common::DisassembleInfo &/*a_info*/,
			  const std::list<common::Asm> &a_asm,
			  bool a_append,
			  const list<UString> &a_src_search_dirs,
			  list<UString> &a_session_dirs,
			  std::map<UString, bool> &a_ignore_paths,
			  Glib::RefPtr<Buffer> &a_buf);


    /// @}

    /// \name signals
    /// @{
    sigc::signal<void,
                 int/*line clicked*/,
                 bool/*dialog requested*/>&
                                 marker_region_got_clicked_signal () const;
    sigc::signal<void, const Gtk::TextBuffer::iterator&>&
                                 insertion_changed_signal () const;
    /// @}
};//end class SourceEditor

NEMIVER_END_NAMESPACE (nemiver)

#endif //__NMV_DBG_SOURCE_EDITOR_H__

