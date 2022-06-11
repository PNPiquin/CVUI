#include "puzzlemaker.h"
#include <iostream>

PuzzleMaker::PuzzleMaker()
: m_grid(),
  m_button("Load"),
  m_input_path_entry(),
  m_image()
{
  // Init private attributes
  input_path = "";

  // Sets the border width of the window.
  set_border_width(10);
  set_default_size(400, 200);

  // When the button receives the "clicked" signal, it will call the
  // on_button_clicked() method defined below.
  m_button.signal_clicked().connect(sigc::mem_fun(*this, &PuzzleMaker::on_button_clicked));

  // This packs the button into the Window (a container).
  m_grid.attach(m_input_path_entry, 0, 0, 3, 1);
  m_grid.attach(m_button, 3, 0, 1, 1);
  m_grid.attach(m_image, 0, 1, 4, 4);
  m_grid.show_all();
  add(m_grid);

  // The final step is to display this newly created widget...
  m_button.show();
}

PuzzleMaker::~PuzzleMaker()
{
}

void PuzzleMaker::on_button_clicked()
{
  input_path = m_input_path_entry.get_buffer()->get_text();
  auto pixbuf = Gdk::Pixbuf::create_from_file(input_path)->scale_simple(400, 200, Gdk::InterpType::INTERP_NEAREST);
  m_image.set(pixbuf);
  std::cout << "Load file: " << input_path << std::endl;
}