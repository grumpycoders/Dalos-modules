#include <stdio.h>
#include "BReadline.h"

using namespace Balau;

Readline::Readline(const String & program, IO<Handle> in)
    : m_in(in)
{
    m_el = el_init(program.to_charp(), stdin, stdout, stderr);
    el_set(m_el, EL_CLIENTDATA, this);
    el_set(m_el, EL_PROMPT, (const char * (*)(EditLine *)) &Readline::elPrompt);
    el_set(m_el, EL_SIGNAL, 1);
    el_set(m_el, EL_GETCFN, (int (*)(EditLine *, char *)) &Readline::elGetCFN);
}

Readline::~Readline() {
    el_end(m_el);
}

void Readline::setPrompt(const String & prompt) {
    m_prompt = prompt;
}

String Readline::gets() {
    int count;
    const char * line = el_gets(m_el, &count);

    if (!line) {
        m_eof = true;
        return "";
    }

    return line;
}

const char * Readline::elPrompt(EditLine * el) {
    Readline * rl;
    el_get(el, EL_CLIENTDATA, &rl);
    return rl->elPrompt();
}

int Readline::elGetCFN(EditLine * el, char * c) {
    Readline * rl;
    el_get(el, EL_CLIENTDATA, &rl);
    return rl->elGetCFN(c);
}

int Readline::elGetCFN(char * c) {
    return m_in->read(c, 1);
}
