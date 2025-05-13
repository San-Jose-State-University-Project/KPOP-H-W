#include "PageManager.h"
#include "I2C.h"

PageManager::PageManager() : currentIndex(0) {}

void PageManager::addPage(const String& title, const String& content) {
  pages.push_back({title, content});
}

void PageManager::nextPage() {
  if (pages.empty()) return;
  currentIndex = (currentIndex + 1) % pages.size();
  showCurrentPage();
}

void PageManager::showCurrentPage() {
  if (pages.empty()) return;
  printLCD(pages[currentIndex].title, pages[currentIndex].content);
}

void PageManager::clearPages() {
  pages.clear();
  currentIndex = 0;
  printLCD("페이지 없음", ""); // or just clear the screen
}