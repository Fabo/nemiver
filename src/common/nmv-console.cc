//Author: Fabien Parent
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

#define USE_VARARGS
#define PREFER_STDARG

#include "nmv-console.h"
#include "nmv-str-utils.h"
#include <map>
#include <vector>
#include <cstring>
#include <cctype>
#include <readline/readline.h>
#include <readline/history.h>

NEMIVER_BEGIN_NAMESPACE(nemiver)
NEMIVER_BEGIN_NAMESPACE(common)

const char *const CONSOLE_PROMPT = "> ";
const unsigned int COMMAND_EXECUTION_TIMEOUT_IN_SECONDS = 10;

struct Console::Stream::Priv {
    int fd;

    Priv (int a_fd) :
        fd (a_fd)
    {
    }

    void
    write (const std::string &a_msg) const
    {
        THROW_IF_FAIL (fd);
        ::write (fd, a_msg.c_str (), a_msg.size ());
    }
};

Console::Stream::Stream (int a_fd) :
    m_priv (new Priv (a_fd))
{
}

Console::Stream&
Console::Stream::operator<< (const char *const a_string)
{
    THROW_IF_FAIL (m_priv);
    m_priv->write (a_string);
    return *this;
}

Console::Stream&
Console::Stream::operator<< (const std::string &a_string)
{
    THROW_IF_FAIL (m_priv);
    m_priv->write (a_string);
    return *this;
}

Console::Stream&
Console::Stream::operator<< (unsigned int a_uint)
{
    THROW_IF_FAIL (m_priv);
    m_priv->write (str_utils::to_string (a_uint));
    return *this;
}

Console::Stream&
Console::Stream::operator<< (int a_int)
{
    THROW_IF_FAIL (m_priv);
    m_priv->write (str_utils::to_string (a_int));
    return *this;
}

struct Console::Priv {
    std::map<std::string, Console::Command&> commands;
    std::vector<Console::Command*> commands_vector;

    int fd;
    struct readline_state console_state;
    struct readline_state saved_state;

    Console::Stream stream;
    Glib::RefPtr<Glib::IOSource> io_source;
    sigc::connection cmd_execution_done_connection;
    sigc::connection cmd_execution_timeout_connection;

    Priv (int a_fd) :
        fd (a_fd),
        stream (a_fd),
        io_source (Glib::IOSource::create (a_fd, Glib::IO_IN))
    {
        init ();
    }

    void
    init ()
    {
        THROW_IF_FAIL (fd);
        THROW_IF_FAIL (io_source);

        if (consoles ().count (fd)) {
            THROW ("Cannot create two consoles from the same file descriptor.");
        }
        consoles ()[fd] = this;

        io_source->connect (sigc::mem_fun (*this, &Console::Priv::read_char));
        io_source->attach ();

        rl_save_state (&saved_state);
        rl_instream = fdopen (fd, "r");
        rl_outstream = fdopen (fd, "w");
        rl_bind_key ('\t', &Console::Priv::on_tab_key_pressed);
        rl_callback_handler_install (CONSOLE_PROMPT,
                                     &Console::Priv::process_command);
        rl_already_prompted = true;
        rl_save_state (&console_state);
        rl_restore_state (&saved_state);
    }

    bool
    read_char (Glib::IOCondition)
    {
        NEMIVER_TRY

        if (cmd_execution_done_connection.connected ())
            return false;

        rl_restore_state (&console_state);
        rl_callback_read_char ();
        rl_save_state (&console_state);
        rl_restore_state (&saved_state);

        NEMIVER_CATCH_NOX

        return true;
    }

    void
    do_completion (const std::string &a_completion)
    {
        size_t buffer_length = std::strlen (rl_line_buffer);
        rl_extend_line_buffer (buffer_length + a_completion.size ());
        std::memcpy (rl_line_buffer + rl_point + a_completion.size (),
                     rl_line_buffer + rl_point,
                     a_completion.size ());
        for (size_t i = 0; i < a_completion.size(); i++) {
            rl_line_buffer[rl_point + i] = a_completion[i];
        }
        rl_end += a_completion.size ();
        rl_point += a_completion.size ();
    }

    void
    do_command_completion (const std::string &a_line)
    {
        std::vector<Console::Command*> matches;
        for (std::vector<Console::Command*>::const_iterator iter =
                commands_vector.begin ();
             iter != commands_vector.end ();
             ++iter) {
            if (*iter && !(*iter)->name ().find (a_line)) {
                matches.push_back (*iter);
            }
        }

        if (matches.size () == 1) {
            std::string completion =
                matches[0]->name ().substr (a_line.size ());
            do_completion (completion);
        } else if (matches.size () > 1) {
            std::string msg;
            std::string completion =
                matches[0]->name ().substr (a_line.size ());
            for (size_t i = 0; i < matches.size (); i++) {
                size_t j = a_line.size ();
                for (; j < matches[i]->name ().size ()
                            && j < completion.size ()
                            && matches[i]->name ()[j] == completion[j];
                     j++) {
                }
                completion = completion.substr (0, j);
                msg += matches[i]->name () + "\t";
            }

            rl_save_prompt ();
            rl_message ("%s%s\n%s\n%s",
                        rl_display_prompt,
                        std::string (rl_line_buffer, rl_end).c_str (),
                        msg.c_str (),
                        rl_display_prompt);
            rl_clear_message ();
            rl_restore_prompt ();
            do_completion (completion);
        }
    }

    void
    do_param_completion (std::vector<UString> &a_tokens)
    {
        if (!a_tokens.size ()) {
            return;
        }

        Console::Command* command = 0;
        for (std::vector<Console::Command*>::const_iterator iter =
                commands_vector.begin ();
             iter != commands_vector.end ();
             ++iter) {
            if (*iter && (*iter)->name () == a_tokens[0]) {
                command = *iter;
                break;
            }
        }

        if (!command) {
            return;
        }

        a_tokens.erase (a_tokens.begin ());
        std::string line (rl_line_buffer, rl_point);
        if (std::isspace (line[line.size () - 1])) {
            stream << "\n";
            command->display_usage (a_tokens, stream);
            rl_forced_update_display ();
        } else {
            UString token = a_tokens.back ();
            a_tokens.pop_back ();

            std::vector<UString> completions;
            std::vector<UString> matches;
            command->completions (a_tokens, completions);
            for (std::vector<UString>::iterator iter = completions.begin ();
                 iter != completions.end ();
                 ++iter) {
                if (!iter->find (token)) {
                    matches.push_back (*iter);
                }
            }

            if (matches.size () == 1) {
                std::string completion = matches[0].substr (token.size ());
                do_completion (completion);
            } else if (matches.size () > 1) {
                std::string msg;
                UString completion = matches[0].substr (token.size ());
                for (size_t i = 0; i < matches.size (); i++) {
                    size_t j = token.size ();
                    for (; j < matches[i].size ()
                                && j < completion.size ()
                                && matches[i][j] == completion[j];
                         j++) {
                    }
                    completion = completion.substr (0, j);
                    msg += matches[i] + "\t";
                }
                rl_save_prompt ();
                rl_message ("%s%s\n%s\n%s",
                            rl_display_prompt,
                            std::string (rl_line_buffer, rl_end).c_str (),
                            msg.c_str (),
                            rl_display_prompt);
                rl_clear_message ();
                rl_restore_prompt ();
                do_completion (completion);
            } else {
                rl_complete (0, '\t');
            }
        }
    }

    void
    execute_command (char *a_buffer)
    {
        std::string command_name;
        std::vector<std::string> cmd_argv;

        int size = std::strlen (a_buffer);
        for (int i = 0, local_index = 0; i <= size; ++i, ++local_index) {
            if (!std::isspace (a_buffer[local_index]) && i != size) {
                continue;
            }

            a_buffer[local_index] = '\0';
            if (command_name.empty ()) {
                command_name = a_buffer;
            } else {
                cmd_argv.push_back (a_buffer);
            }
            a_buffer += local_index + 1;
            local_index = 0;
        }

        if (command_name.empty ()) {
            stream << CONSOLE_PROMPT;
            return;
        }

        if (!commands.count (command_name)) {
            stream << "Undefined command: " << command_name << ".\n"
                   << CONSOLE_PROMPT;
            return;
        }

        Command &command = commands.at (command_name);
        cmd_execution_done_connection = command.done_signal ().connect
            (sigc::mem_fun (*this, &Console::Priv::on_done_signal));
        commands.at (command_name) (cmd_argv, stream);
        cmd_execution_timeout_connection =
            Glib::signal_timeout().connect_seconds
                (sigc::mem_fun
                    (*this, &Console::Priv::on_cmd_execution_timeout_signal),
                 COMMAND_EXECUTION_TIMEOUT_IN_SECONDS);
    }

    bool
    on_cmd_execution_timeout_signal ()
    {
        NEMIVER_TRY
        on_done_signal ();
        NEMIVER_CATCH_NOX

        return true;
    }

    void
    on_done_signal ()
    {
        NEMIVER_TRY

        stream << CONSOLE_PROMPT;
        cmd_execution_timeout_connection.disconnect();
        cmd_execution_done_connection.disconnect ();

        NEMIVER_CATCH_NOX
    }

    static int
    on_tab_key_pressed (int, int)
    {
        NEMIVER_TRY

        std::string line (rl_line_buffer, rl_point);
        std::vector<UString> tokens_raw = str_utils::split (line, " ");
        std::vector<UString> tokens;
        for (size_t i = 0; i < tokens_raw.size (); i++) {
            if (tokens_raw[i].size ()) {
                tokens.push_back (tokens_raw[i]);
            }
        }

        if (std::isspace (line[line.size () - 1]) || tokens.size () > 1) {
            self ().do_param_completion (tokens);
        } else {
            self ().do_command_completion (line);
        }

        NEMIVER_CATCH_NOX

        return 0;
    }

    static std::map<int, Console::Priv*>& consoles ()
    {
        static std::map<int, Console::Priv*> s_consoles;
        return s_consoles;
    }

    static Console::Priv& self ()
    {
        int fd = fileno (rl_instream);
        THROW_IF_FAIL (fd);
        THROW_IF_FAIL (consoles ().count (fd));
        Console::Priv *self = consoles ()[fd];
        THROW_IF_FAIL (self);
        return *self;
    }

    static void
    process_command (char *a_command)
    {
        NEMIVER_TRY

        THROW_IF_FAIL (a_command);
        add_history (a_command);
        self ().execute_command (a_command);

        NEMIVER_CATCH_NOX

        free (a_command);
    }
};

Console::Console (int a_fd) :
    m_priv (new Priv (a_fd))
{
}

Console::~Console ()
{
}

void
Console::register_command (Console::Command &a_command)
{
    THROW_IF_FAIL (m_priv);

    if (m_priv->commands.count (a_command.name ())) {
        LOG ("Command '" << a_command.name () << "' is already registered in"
             " the console. The previous command will be overwritten");
    }

    m_priv->commands.insert (std::make_pair<std::string, Command&>
        (a_command.name (), a_command));
    m_priv->commands_vector.push_back (&a_command);

    const char **aliases = a_command.aliases ();
    for (int i = 0; aliases && aliases[i]; i++) {
        if (m_priv->commands.count (aliases[i])) {
            LOG ("Command '" << aliases[i] << "' is already registered in"
                 " the console. The previous command will be overwritten");
        }
        m_priv->commands.insert (std::make_pair<std::string, Command&>
            (aliases[i], a_command));
    }
}

NEMIVER_END_NAMESPACE(common)
NEMIVER_END_NAMESPACE(nemiver)

