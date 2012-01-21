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

#include "nmv-dbg-console.h"
#include "nmv-i-debugger.h"
#include "uicommon/nmv-terminal.h"
#include "common/nmv-exception.h"
#include "common/nmv-str-utils.h"
#include "common/nmv-ustring.h"

NEMIVER_BEGIN_NAMESPACE(nemiver)

const char *const NEMIVER_DBG_CONSOLE_COOKIE = "nemiver-dbg-console";

using namespace common;

struct DebuggingData {
    IDebugger &debugger;
    IDebugger::Frame current_frame;
    UString current_file_path;
    std::vector<UString> source_files;

    DebuggingData (IDebugger &a_debugger) :
        debugger (a_debugger)
    {
    }
};

struct CommandContinue : public Console::SynchronousCommand {
    IDebugger &debugger;

    CommandContinue (IDebugger &a_debugger) :
        debugger (a_debugger)
    {
    }

    const std::string&
    name () const
    {
        static const std::string &s_name = "continue";
        return s_name;
    }

    const char**
    aliases () const
    {
        static const char *s_aliases[] =
        {
            "c",
            0
        };
        return s_aliases;
    }

    void
    execute (const std::vector<std::string>&, Console::Stream&)
    {
        debugger.do_continue ();
    }
};

struct CommandNext : public Console::SynchronousCommand {
    IDebugger &debugger;

    CommandNext (IDebugger &a_debugger) :
        debugger (a_debugger)
    {
    }

    const std::string&
    name () const
    {
        static const std::string &s_name = "next";
        return s_name;
    }

    const char**
    aliases () const
    {
        static const char *s_aliases[] =
        {
            "n",
            0
        };
        return s_aliases;
    }

    void
    execute (const std::vector<std::string>&, Console::Stream&)
    {
        debugger.step_over ();
    }
};

struct CommandStep : public Console::SynchronousCommand {
    IDebugger &debugger;

    CommandStep (IDebugger &a_debugger) :
        debugger (a_debugger)
    {
    }

    const std::string&
    name () const
    {
        static const std::string &s_name = "step";
        return s_name;
    }

    const char**
    aliases () const
    {
        static const char *s_aliases[] =
        {
            "s",
            0
        };
        return s_aliases;
    }

    void
    execute (const std::vector<std::string>&,
             Console::Stream&)
    {
        debugger.step_in ();
    }
};

struct CommandNexti : public Console::SynchronousCommand {
    IDebugger &debugger;

    CommandNexti (IDebugger &a_debugger) :
        debugger (a_debugger)
    {
    }

    const std::string&
    name () const
    {
        static const std::string &s_name = "nexti";
        return s_name;
    }

    const char**
    aliases () const
    {
        static const char *s_aliases[] =
        {
            "ni",
            0
        };
        return s_aliases;
    }

    void
    execute (const std::vector<std::string>&, Console::Stream&)
    {
        debugger.step_over_asm ();
    }
};

struct CommandStepi : public Console::SynchronousCommand {
    IDebugger &debugger;

    CommandStepi (IDebugger &a_debugger) :
        debugger (a_debugger)
    {
    }

    const std::string&
    name () const
    {
        static const std::string &s_name = "stepi";
        return s_name;
    }

    const char**
    aliases () const
    {
        static const char *s_aliases[] =
        {
            "si",
            0
        };
        return s_aliases;
    }

    void
    execute (const std::vector<std::string>&, Console::Stream&)
    {
        debugger.step_in_asm ();
    }
};

struct CommandStop : public Console::SynchronousCommand {
    IDebugger &debugger;

    CommandStop (IDebugger &a_debugger) :
        debugger (a_debugger)
    {
    }

    const std::string&
    name () const
    {
        static const std::string &s_name = "stop";
        return s_name;
    }

    void
    execute (const std::vector<std::string>&, Console::Stream&)
    {
        debugger.stop_target ();
    }
};

struct CommandFinish : public Console::SynchronousCommand {
    IDebugger &debugger;

    CommandFinish (IDebugger &a_debugger) :
        debugger (a_debugger)
    {
    }

    const std::string&
    name () const
    {
        static const std::string &s_name = "finish";
        return s_name;
    }

    void
    execute (const std::vector<std::string>&, Console::Stream&)
    {
        debugger.step_out ();
    }
};

struct CommandCall : public Console::SynchronousCommand {
    IDebugger &debugger;
    std::string cmd;

    CommandCall (IDebugger &a_debugger) :
        debugger (a_debugger)
    {
    }

    const std::string&
    name () const
    {
        static const std::string &s_name = "call";
        return s_name;
    }

    void
    execute (const std::vector<std::string> &a_argv, Console::Stream &a_stream)
    {
        if (a_argv.size ()) {
            cmd.clear ();
        }

        for (std::vector<std::string>::const_iterator iter = a_argv.begin ();
             iter != a_argv.end ();
             ++iter) {
            cmd += *iter;
        }

        if (cmd.empty ()) {
            a_stream << "The history is empty.\n";
        } else {
            debugger.call_function (cmd);
        }
    }
};

struct CommandThread : public Console::AsynchronousCommand {
    IDebugger &debugger;

    CommandThread (IDebugger &a_debugger) :
        debugger (a_debugger)
    {
    }

    const std::string&
    name () const
    {
        static const std::string &s_name = "thread";
        return s_name;
    }

    void completions (const std::vector<UString> &a_argv,
                      std::vector<UString> &a_completion_vector) const
    {
        if (a_argv.size () == 0) {
            a_completion_vector.push_back ("list");
        }
    }

    void display_usage (const std::vector<UString> &a_argv,
                        Console::Stream &a_stream) const
    {
        if (a_argv.size ()) {
            return;
        }

        a_stream << "Usage:\n"
                 << "\tthread\n"
                 << "\tthread [THREAD ID]\n"
                 << "\tthread list\n";
    }

    void
    threads_listed_signal (const std::list<int> a_thread_ids,
                           const UString &a_cookie,
                           Console::Stream &a_stream)
    {
        NEMIVER_TRY

        if (a_cookie != NEMIVER_DBG_CONSOLE_COOKIE) {
            return;
        }

        a_stream << "Threads:\n";
        for (std::list<int>::const_iterator iter = a_thread_ids.begin ();
             iter != a_thread_ids.end ();
             ++iter) {
            a_stream << *iter << "\n";
        }
        done_signal().emit ();

        NEMIVER_CATCH_NOX
    }

    void
    execute (const std::vector<std::string> &a_argv, Console::Stream &a_stream)
    {
        if (a_argv.size () > 1) {
            a_stream << "Too much parameters.\n";
        } else if (!a_argv.size ()) {
            a_stream << "Current thread ID: " << debugger.get_current_thread ()
                     << ".\n";
        } else if (str_utils::string_is_number (a_argv[0])) {
            debugger.select_thread
                (str_utils::from_string<unsigned int> (a_argv[0]));
        } else if (a_argv[0] == "list") {
            debugger.threads_listed_signal ().connect
                (sigc::bind<Console::Stream&> (sigc::mem_fun
                    (*this, &CommandThread::threads_listed_signal),
                 a_stream));
            debugger.list_threads (NEMIVER_DBG_CONSOLE_COOKIE);
            return;
        } else {
            a_stream << "Invalid argument: " << a_argv[0] << ".\n";
        }

        done_signal().emit ();
    }
};

struct CommandBreak : public Console::SynchronousCommand {
    DebuggingData &dbg_data;

    CommandBreak (DebuggingData &a_dbg_data) :
        dbg_data (a_dbg_data)
    {
    }

    const std::string&
    name () const
    {
        static const std::string &s_name = "break";
        return s_name;
    }

    const char**
    aliases () const
    {
        static const char *s_aliases[] =
        {
            "b",
            0
        };
        return s_aliases;
    }

    void display_usage (const std::vector<UString>&,
                        Console::Stream &a_stream) const
    {
        a_stream << "Usage:\n"
                 << "\tbreak\n"
                 << "\tbreak [LINE]\n"
                 << "\tbreak [FUNCTION]\n"
                 << "\tbreak *[ADDRESS]\n"
                 << "\tbreak +[OFFSET]\n"
                 << "\tbreak -[OFFSET]\n";
    }

    void
    execute (const std::vector<std::string> &a_argv, Console::Stream &a_stream)
    {
        IDebugger::Frame &frame = dbg_data.current_frame;
        IDebugger &debugger = dbg_data.debugger;

        if (frame.file_full_name ().empty () ||
            dbg_data.current_file_path.empty ()) {
            a_stream << "Cannot set a breakpoint at this position.\n";
        }

        if (a_argv.size () > 1) {
            a_stream << "Too much parameters.\n";
        } else if (a_argv.size () == 0) {
            debugger.set_breakpoint (frame.file_full_name (), frame.line ());
        }

        const char first_param_char = a_argv[0][0];
        if (str_utils::string_is_number (a_argv[0])) {
            debugger.set_breakpoint (dbg_data.current_file_path,
                                     str_utils::from_string<int> (a_argv[0]));
        } else if ((first_param_char >= 'a' && first_param_char <= 'z')
                   || first_param_char == '_') {
            debugger.set_breakpoint (a_argv[0]);
        } else if (first_param_char == '*') {
            std::string addr (a_argv[0].substr (1));
            if (str_utils::string_is_hexa_number (addr)) {
                Address address (addr);
                debugger.set_breakpoint (address);
            } else {
                a_stream << "Invalid address: " << addr << ".\n";
            }
        } else if (first_param_char == '+' || first_param_char == '-') {
            std::string offset (a_argv[0].substr (1));
            if (str_utils::string_is_decimal_number (offset)) {
                int line = frame.line ();
                if (first_param_char == '+') {
                    line += str_utils::from_string<int> (offset);
                } else {
                    line -= str_utils::from_string<int> (offset);
                }
                debugger.set_breakpoint (frame.file_full_name (), line);
            } else {
                a_stream << "Invalid offset: " << offset << ".\n";
            }
        } else {
            a_stream << "Invalid argument: " << a_argv[0] << ".\n";
        }
    }
};

struct CommandPrint : public Console::AsynchronousCommand {
    IDebugger &debugger;
    std::string expression;

    CommandPrint (IDebugger &a_debugger) :
        debugger (a_debugger)
    {
    }

    const std::string&
    name () const
    {
        static const std::string &s_name = "print";
        return s_name;
    }

    void
    on_variable_created_signal (const IDebugger::VariableSafePtr a_var,
                                Console::Stream &a_stream)
    {
        NEMIVER_TRY

        THROW_IF_FAIL (a_var);
        a_stream << a_var->name () << " = " << a_var->value () << "\n";
        debugger.delete_variable (a_var);
        done_signal ().emit ();

        NEMIVER_CATCH_NOX
    }

    void
    execute (const std::vector<std::string> &a_argv, Console::Stream &a_stream)
    {
        if (a_argv.size ()) {
            expression.clear ();
        }

        for (std::vector<std::string>::const_iterator iter = a_argv.begin ();
             iter != a_argv.end ();
             ++iter) {
            expression += *iter;
        }

        if (expression.empty ()) {
            a_stream << "No history\n";
            done_signal ().emit ();
            return;
        }

        debugger.create_variable
            (expression, sigc::bind<Console::Stream&>
                (sigc::mem_fun
                    (*this, &CommandPrint::on_variable_created_signal),
                 a_stream));
    }
};

struct CommandOpen : public Console::SynchronousCommand {
    DebuggingData &dbg_data;
    sigc::signal<void, UString> file_opened_signal;

    CommandOpen (DebuggingData &a_dbg_data) :
        dbg_data (a_dbg_data)
    {
    }

    const std::string&
    name () const
    {
        static const std::string &s_name = "open";
        return s_name;
    }

    const char**
    aliases () const
    {
        static const char *s_aliases[] =
        {
            "o",
            0
        };
        return s_aliases;
    }

    void completions (const std::vector<UString>&,
                      std::vector<UString> &a_completion_vector) const
    {
        a_completion_vector.insert (a_completion_vector.begin (),
                                    dbg_data.source_files.begin (),
                                    dbg_data.source_files.end ());
    }

    void
    execute (const std::vector<std::string> &a_argv, Console::Stream&)
    {
        for (std::vector<std::string>::const_iterator iter = a_argv.begin ();
             iter != a_argv.end ();
             ++iter) {
            UString path = *iter;
            if (path.size () && path[0] == '~') {
                path = path.replace (0, 1, Glib::get_home_dir ());
            }
            file_opened_signal.emit (path);
        }
    }
};

struct DBGConsole::Priv {
    DebuggingData data;

    CommandContinue cmd_continue;
    CommandNext cmd_next;
    CommandStep cmd_step;
    CommandBreak cmd_break;
    CommandPrint cmd_print;
    CommandCall cmd_call;
    CommandFinish cmd_finish;
    CommandThread cmd_thread;
    CommandStop cmd_stop;
    CommandNexti cmd_nexti;
    CommandStepi cmd_stepi;
    CommandOpen cmd_open;

    Priv (IDebugger &a_debugger) :
        data (a_debugger),
        cmd_continue (a_debugger),
        cmd_next (a_debugger),
        cmd_step (a_debugger),
        cmd_break (data),
        cmd_print (a_debugger),
        cmd_call (a_debugger),
        cmd_finish (a_debugger),
        cmd_thread (a_debugger),
        cmd_stop (a_debugger),
        cmd_nexti (a_debugger),
        cmd_stepi (a_debugger),
        cmd_open (data)
    {
        init_signals ();
    }

    void
    on_stopped_signal (IDebugger::StopReason,
                       bool,
                       const IDebugger::Frame &a_frame,
                       int,
                       int,
                       const UString&)
    {
        data.current_frame = a_frame;
        data.current_file_path = a_frame.file_full_name ();
    }

    void
    on_files_listed_signal (const std::vector<UString> &a_files, const UString&)
    {
        data.source_files = a_files;
    }

    void
    init_signals ()
    {
        data.debugger.stopped_signal ().connect
            (sigc::mem_fun (*this, &DBGConsole::Priv::on_stopped_signal));
        data.debugger.files_listed_signal ().connect (sigc::mem_fun
            (*this, &DBGConsole::Priv::on_files_listed_signal));
    }
};

DBGConsole::DBGConsole (int a_fd, IDebugger &a_debugger) :
    Console (a_fd),
    m_priv (new Priv (a_debugger))
{
    register_command (m_priv->cmd_continue);
    register_command (m_priv->cmd_next);
    register_command (m_priv->cmd_step);
    register_command (m_priv->cmd_break);
    register_command (m_priv->cmd_print);
    register_command (m_priv->cmd_call);
    register_command (m_priv->cmd_finish);
    register_command (m_priv->cmd_thread);
    register_command (m_priv->cmd_stop);
    register_command (m_priv->cmd_nexti);
    register_command (m_priv->cmd_stepi);
    register_command (m_priv->cmd_open);
}

void
DBGConsole::current_file_path (const UString &a_file_path)
{
    THROW_IF_FAIL (m_priv);
    m_priv->data.current_file_path = a_file_path;
}

const UString&
DBGConsole::current_file_path () const
{
    THROW_IF_FAIL (m_priv);
    return m_priv->data.current_file_path;
}

DBGConsole::~DBGConsole ()
{
}

sigc::signal<void, UString>
DBGConsole::file_opened_signal () const
{
    THROW_IF_FAIL (m_priv);
    return m_priv->cmd_open.file_opened_signal;
}

NEMIVER_END_NAMESPACE(nemiver)

