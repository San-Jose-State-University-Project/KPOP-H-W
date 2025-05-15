#include "PageManager.h"
#include "I2C.h"

PageManager::PageManager() : currentIndex(0) {}

void PageManager::addPage(const String& title, const String& content) {
  pages.push_back({title, content});
  maxPage++;
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
  maxPage = 0;
  printLCD("Empty", "Pages"); // or just clear the screen
}

size_t PageManager::getMaxPage() {
  return maxPage;
}
