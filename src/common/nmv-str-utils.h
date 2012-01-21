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
#ifndef __NMV_STR_UTILS_H__
#define __NMV_STR_UTILS_H__
#include "nmv-ustring.h"
#include <sstream>

NEMIVER_BEGIN_NAMESPACE (nemiver)
NEMIVER_BEGIN_NAMESPACE (str_utils)

using nemiver::common::UString;

bool extract_path_and_line_num_from_location (const std::string &a_location,
					      std::string &a_file_path,
					      std::string &a_line_num);

bool parse_host_and_port (const std::string &a,
			  std::string &a_host,
			  unsigned &a_port);

size_t hexa_to_int (const string &a_hexa_str);
std::string int_to_string (size_t an_int);
bool string_is_number (const string&);
bool string_is_decimal_number (const string&);
bool string_is_hexa_number (const string &a_str);
vector<UString> split (const UString &a_string, const UString &a_delim);
vector<UString> split_set (const UString &a_string, const UString &a_delim_set);
UString join (const vector<UString> &a_elements,
              const UString &a_delim=" ");
UString join (vector<UString>::const_iterator &a_from,
              vector<UString>::const_iterator &a_to,
              const UString &a_delim=" ");

template<typename T>
T
from_string (const std::string &a_string)
{
    std::istringstream is (a_string);
    T out;
    is >> out;
    return out;
}

template<typename T>
std::string
to_string (const T &a_value)
{
    std::ostringstream os;
    os << a_value;
    return os.str ();
}

template<typename S>
void
chomp (S &a_string)
{
    if (!a_string.size ()) {return;}

    Glib::ustring::size_type i = 0;

    // remove the ws from the beginning of the string.
    while (!a_string.empty () && isspace (a_string.at (0))) {
        a_string.erase (0, 1);
    }

    // remove the ws from the end of the string.
    i = a_string.size ();
    if (!i) {return;}
    --i;
    while (i > 0 && isspace (a_string.at (i))) {
        a_string.erase (i, 1);
        i = a_string.size ();
        if (!i) {return;}
        --i;
    }
    if (i == 0 && isspace (a_string.at (i))) {a_string.erase (0, 1);}
}

UString::size_type get_number_of_lines (const UString &a_string);

UString::size_type get_number_of_words (const UString &a_string);

UString printf (const UString &a_format, ...);

UString vprintf (const UString &a_format, va_list a_args);

bool is_buffer_valid_utf8 (const char *a_buffer, unsigned a_len);

bool ensure_buffer_is_in_utf8 (const std::string &a_input,
			       const std::list<std::string> &supported_encodings,
			       UString &a_output);
			       

NEMIVER_END_NAMESPACE (str_utils)
NEMIVER_END_NAMESPACE (nemiver)

#endif // __NMV_STR_UTILS_H__
