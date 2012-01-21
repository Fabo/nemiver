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
#ifndef __NMV_CONSOLE_H__
#define __NMV_CONSOLE_H__

#include "nmv-safe-ptr.h"
#include "nmv-namespace.h"
#include "nmv-exception.h"
#include <string>
#include <vector>

NEMIVER_BEGIN_NAMESPACE(nemiver)
NEMIVER_BEGIN_NAMESPACE(common)

class Console {
    struct Priv;
    SafePtr<Priv> m_priv;

public:
    class Stream {
        struct Priv;
        SafePtr<Priv> m_priv;
        
    public:
        explicit Stream (int a_fd);
        Stream& operator<< (const char *const a_string);
        Stream& operator<< (const std::string &a_string);
        Stream& operator<< (unsigned int a_uint);
        Stream& operator<< (int a_int);
    };

    class Command {
        sigc::signal<void> m_done_signal;

public:
        sigc::signal<void>& done_signal ()
        {
            return m_done_signal;
        }

        virtual const std::string& name () const = 0;

        virtual const char** aliases () const
        {
            return 0;
        }

        virtual void completions (const std::vector<UString>&,
                                  std::vector<UString>&) const
        {
        }

        virtual void display_usage (const std::vector<UString>&, Stream&) const
        {
        }

        virtual void execute (const std::vector<std::string> &a_argv,
                              Stream &a_output) = 0;

        virtual void operator() (const std::vector<std::string> &a_argv,
                         Stream &a_output)
        {
            execute (a_argv, a_output);
        }

        virtual ~Command ()
        {
        }
    };

    typedef Command AsynchronousCommand;
    struct SynchronousCommand : public Command{
        virtual void operator() (const std::vector<std::string> &a_argv,
                         Stream &a_output)
        {
            execute (a_argv, a_output);
            done_signal ().emit ();
        }
    };

    explicit Console (int a_fd);
    virtual ~Console ();
    void register_command (Console::Command &a_command);
};

NEMIVER_END_NAMESPACE(common)
NEMIVER_END_NAMESPACE(nemiver)

#endif /* __NMV_CONSOLE_H__ */

