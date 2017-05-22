#ifndef REPLYTODEVTRANSLATOR_H
#define REPLYTODEVTRANSLATOR_H

#include "icmdtranslator.h"

class ReplyToDevTranslator : public ICmdTranslator
{
public:
    ReplyToDevTranslator();

    QByteArray & translate(QByteArray &aDest, const QByteArray &aSrc);

};

#endif // REPLYTODEVTRANSLATOR_H
