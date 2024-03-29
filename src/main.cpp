#include "cvui.h"
#include <gtkmm/application.h>

int main(int argc, char* argv[])
{
  auto app = Gtk::Application::create("com.pnp.cvui");

  CVUI cvui;

  // Shows the window and returns when it is closed.
  return app->make_window_and_run<CVUI>(argc, argv);
}