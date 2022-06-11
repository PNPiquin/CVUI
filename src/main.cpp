#include "puzzlemaker.h"
#include <gtkmm/application.h>

int main(int argc, char* argv[])
{
  auto app = Gtk::Application::create(argc, argv, "com.pnp.puzzlemaker");

  PuzzleMaker puzzlemaker;

  // Shows the window and returns when it is closed.
  return app->run(puzzlemaker);
}