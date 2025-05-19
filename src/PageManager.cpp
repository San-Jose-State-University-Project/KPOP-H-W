#include "PageManager.h"
#include "I2C.h"
#include "LedManager.h"

PageManager::PageManager(LedManager *lm) : currentIndex(0) {
  ledManager = lm;
}

void PageManager::addPage(const String& title, const String& content, const String& emotion) {
  pages.push_back({title, content, emotion});
  maxPage++;
}

void PageManager::nextPage() {
  if (pages.empty()) return;
  currentIndex = (currentIndex + 1) % pages.size();
  showCurrentPage();
}

void PageManager::beforePage() {
  if (pages.empty()) return;
  currentIndex = (currentIndex + pages.size() - 1) % pages.size();
  showCurrentPage();
}

void PageManager::showCurrentPage() {
  if (pages.empty()) return;
  printLCD(pages[currentIndex].title, pages[currentIndex].content);
  ledManager->setEmotion(pages[currentIndex].emotion);
}

void PageManager::clearPages() {
  pages.clear();
  currentIndex = 0;
  maxPage = 0;
  printLCD("Empty", "Pages"); // or just clear the screen
}

size_t PageManager::getCurrentPage() {
  return currentIndex;
}

size_t PageManager::getMaxPage() {
  return maxPage;
}
