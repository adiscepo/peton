#include <gtkmm.h>
#include <iostream>

class Peton : public Gtk::Window {
protected:
    void on_button_clicked();
    Gtk::Button m_button;
public:
  Peton();
  ~Peton() override {};
};

Peton::Peton() : m_button("Lancer la simulation")  {
  set_title("Basic application");
  set_default_size(1400, 900);
  m_button.set_margin(10);
  m_button.signal_clicked().connect(sigc::mem_fun(*this, &Peton::on_button_clicked));
  set_child(m_button);
}

void Peton::on_button_clicked() {
  std::cout << "Comment ça va ?" << std::endl;
}

int main(int argc, char* argv[]) {
  auto app = Gtk::Application::create("org.gtkmm.examples.base");

  return app->make_window_and_run<Peton>(argc, argv);
}
