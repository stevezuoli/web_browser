#include "Evernote/evernote_manager.h"

using namespace evernote_kindle;

int main()
{
    KindleAnnotationItem item1;
    KindleAnnotationItem item2;
    
    QString chapter1 = "chapter 1";
    QString chapter2 = "chapter 2";
    item1.chapter_id = chapter1;
    item1.content = "Reference content 1";
    item1.comment = "My comment 1";
    item1.create_time = "2013-05-24T10:15:07";
    
    item2.chapter_id = chapter2;
    item2.content = "Reference content 2";
    item2.comment = "My comment 2";
    item2.create_time = "2013-05-25T09:20:25";
    
    EvernoteContent note;
    note.book_id = "u65g18c77fbccf78bfd2240f1g48b148";
    note.author = "Steve Zuo";
    note.chapters.insert(chapter1);
    note.chapters.insert(chapter2);
    
    note.annotations.insert(chapter1, item1);
    note.annotations.insert(chapter2, item2);
    note.title = "Test Book";

    QString title;
    QString content;
    QString header;
    EvernoteManager note_manager;
    note_manager.prepareContent(note, title, header, content);
    
    QFile file("test.html");
    if (file.open(QIODevice::WriteOnly))
    {
        QString output = header.toLocal8Bit().constData();
        output = output + content.toUtf8().constData();
        file.write(output.toUtf8().constData());
    }
    file.close();
    return 0;
}

