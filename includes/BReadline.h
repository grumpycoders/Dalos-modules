#pragma once

#ifdef USE_EDITLINE
#include <BString.h>
#include <Handle.h>

class Readline {
  public:
      Readline(const Balau::String & program);
      ~Readline();
      Balau::String gets();
      bool gotEOF();
      void setPrompt(const Balau::String & prompt);
};

#endif

#ifdef USE_HISTEDIT
#include <histedit.h>
#include <BString.h>
#include <Handle.h>

class Readline {
  public:
      Readline(const Balau::String & program);
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

#endif
