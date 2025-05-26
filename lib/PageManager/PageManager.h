#ifndef PAGEMANAGER_H
#define PAGEMANAGER_H

#include <Arduino.h>
#include <vector>

#include <LedManager.h>

class PageManager {
public:
  PageManager(LedManager *lm);
  void addPage(const String& title, const String& content, const String& emotion = "neutral");
  void nextPage();
  void beforePage();
  void showCurrentPage();
  void clearPages();
  size_t getCurrentPage();
  size_t getMaxPage();
private:
  struct Page {
    String title;
    String content;
    String emotion;
  };
  std::vector<Page> pages;
  LedManager *ledManager;
  size_t currentIndex;
  size_t maxPage;
};

#endif