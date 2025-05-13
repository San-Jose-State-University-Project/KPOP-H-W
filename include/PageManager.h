#ifndef PAGEMANAGER_H
#define PAGEMANAGER_H

#include <Arduino.h>
#include <vector>

class PageManager {
public:
  PageManager();
  void addPage(const String& title, const String& content);
  void nextPage();
  void showCurrentPage();
  void clearPages();
private:
  struct Page {
    String title;
    String content;
  };

  std::vector<Page> pages;
  size_t currentIndex;
};

#endif