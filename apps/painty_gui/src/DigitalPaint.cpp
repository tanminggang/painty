#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <QApplication>
#include <QGraphicsView>
#include <memory>

#include "DigitalPaintMainWindow.h"

int main(int argc, char** args) {
  auto* app = new QApplication(argc, args);

  DigitalPaintMainWindow view(nullptr);
  view.showMaximized();

  return app->exec();
}