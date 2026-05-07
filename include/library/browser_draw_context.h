#pragma once

#include <vector>

class Text;
class Book;
class Button;

struct BrowserDrawContext {
    Text *ts;
    const std::vector<Book *> *books;
    Book *selected_book;
    const std::vector<Button *> *buttons;
};
