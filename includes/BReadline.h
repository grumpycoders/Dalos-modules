#pragma once

#include <histedit.h>
#include <BString.h>
#include <Handle.h>

class Readline {
  public:
      Readline(const Balau::String & program, Balau::IO<Balau::Handle>);
      ~Readline();
    Balau::String gets();
    bool gotEOF() { return m_eof; }
    void setPrompt(const Balau::String & prompt);
  private:
    EditLine * m_el = NULL;
    History * m_hist = NULL;
    Balau::IO<Balau::Handle> m_in;
    bool m_eof = false;
    Balau::String m_prompt = "> ";

    static const char * elPrompt(EditLine *);
    const char * elPrompt() { return m_prompt.to_charp(); }
    static int elGetCFN(EditLine *, char * c);
    int elGetCFN(char * c);
};
