
#include <gtkmm.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <iomanip>

// --- CORE LOGIC: INDIVIDUAL TASK (LR9) ---

/**
 * @brief Структура для хранения результатов анализа текста.
 */
struct TextAnalysisResult {
    int char_count = 0;
    int word_count = 0;
    int sentence_count = 0;
    double avg_word_length = 0.0;
    std::vector<std::pair<std::string, int>> top_words;
    std::string error_message;
};

/**
 * @brief Выполняет частотный и статистический анализ текста. (ЛР9)
 */
TextAnalysisResult analyze_text(const std::string& text) {
    TextAnalysisResult result;

    if (text.empty()) {
        result.error_message = "Ошибка: Введенный текст пуст.";
        return result;
    }

    result.char_count = text.size();

    for (size_t i = 0; i < text.size(); ++i) {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?') {
            if (i + 1 == text.size() || isspace(text[i + 1])) {
                result.sentence_count++;
            }
        }
    }
    if (result.sentence_count == 0 && !text.empty()) {
        result.sentence_count = 1;
    }

    std::string clean_text = text;
    std::transform(clean_text.begin(), clean_text.end(), clean_text.begin(),
        [](unsigned char c){ return std::tolower(c); });

    std::string temp_clean;
    for (char c : clean_text) {
        if (std::isalnum(c) || std::isspace(c) || c == '-') {
            temp_clean += c;
        } else {
            temp_clean += ' ';
        }
    }
    clean_text = temp_clean;

    std::stringstream ss(clean_text);
    std::string word;
    std::map<std::string, int> word_freq;
    long long total_word_length = 0;
    int valid_word_count = 0;

    while (ss >> word) {
        if (word.length() > 2) { 
            word_freq[word]++;
            total_word_length += word.length();
            valid_word_count++;
        }
        result.word_count++;
    }

    if (valid_word_count > 0) {
        result.avg_word_length = static_cast<double>(total_word_length) / valid_word_count;
    }

    for (const auto& pair : word_freq) {
        result.top_words.push_back(pair);
    }
    
    std::sort(result.top_words.begin(), result.top_words.end(), 
        [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
        return a.second > b.second;
    });

    if (result.top_words.size() > 10) {
        result.top_words.resize(10);
    }

    return result;
}

// --- GTKMM APPLICATION CLASS ---

class LRAppWindow : public Gtk::ApplicationWindow {
public:
    LRAppWindow(Glib::RefPtr<Gtk::Application>& app);
    virtual ~LRAppWindow();

private:
    // Главные контейнеры
    Gtk::Stack m_Stack;
    Gtk::ScrolledWindow m_ScrolledWindow;
    Gtk::HeaderBar m_HeaderBar;

    // Страницы
    Gtk::Widget* m_MainMenu;
    Gtk::Widget* m_LRView;
    Gtk::Widget* m_LR9View;

    // Элементы для LR9 (Индивидуальное Задание)
    Gtk::TextView m_LR9InputText;
    Gtk::Label m_LR9ResultsLabel;
    Gtk::Button m_LR9AnalyzeButton;
    Gtk::Button m_LR9LoadFileButton;
    Gtk::Button m_LR9BackButton;

    // Текущий контекст ЛР (например, "ЛР №1", "ЛР №2, Задание 1")
    std::string m_current_lr_context;
    std::string m_current_laba_executable; // laba1, laba2_1, laba2_2, etc.

    // --- Методы построения интерфейса ---
    Gtk::Widget* build_main_menu();
    Gtk::Widget* build_lr_template_view(const std::string& title);
    Gtk::Widget* build_lr9_view();

    // --- Обработчики сигналов ---
    void on_main_menu_button_click(int lr_number, int task_number = 0);
    void display_results_dialog(const std::string& title, const std::string& result_text);
    void on_input_mode_selected(const std::string& mode);
    void on_back_button_click();
    void on_lr9_analyze_click();
    void on_lr9_load_file_click();

    // --- Асинхронные обработчики диалогов ---
    void on_file_chooser_response_lr_template(int response_id, Gtk::FileChooserDialog* dialog);
    void on_manual_input_dialog_response(int response_id, Gtk::Dialog* dialog, Glib::RefPtr<Gtk::TextBuffer> buffer);

    // --- Логика выполнения ---
    std::string get_laba_executable_name(int lr_number, int task_number);
    void show_manual_input_dialog();
    std::string simulate_external_laba_execution(const std::string& input_source, const std::string& data);
};

LRAppWindow::LRAppWindow(Glib::RefPtr<Gtk::Application>& app)
    : Gtk::ApplicationWindow(app) 
{
    set_title("Консолидированные Лабораторные Работы (C++ / GTKmm)");
    set_default_size(800, 600);

    set_title_bar("Главное меню");

    m_ScrolledWindow.set_child(m_Stack);
    m_ScrolledWindow.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);
    set_child(m_ScrolledWindow);

    m_MainMenu = build_main_menu();
    m_Stack.add(*m_MainMenu, "main_menu", "Главное меню");

    m_LRView = build_lr_template_view("Выберите метод ввода");
    m_Stack.add(*m_LRView, "lr_template", "Выбор ввода");

    m_LR9View = build_lr9_view();
    m_Stack.add(*m_LR9View, "lr9_view", "Индивидуальное Задание");

    m_Stack.set_visible_child("main_menu");
}

LRAppWindow::~LRAppWindow() {}

// --- Вспомогательные методы ---

std::string LRAppWindow::get_laba_executable_name(int lr_number, int task_number) {
    if (lr_number == 2 && task_number == 1) return "laba2_1";
    if (lr_number == 2 && task_number == 2) return "laba2_2";
    if (lr_number == 9) return "laba9"; // Имитация для LR9
    return "laba" + std::to_string(lr_number);
}

void LRAppWindow::set_title_bar(const std::string& title) {
    set_title(title);

    m_LR9BackButton.set_label("ᐊ Назад");
    m_LR9BackButton.signal_clicked().connect(sigc::mem_fun(*this, &LRAppWindow::on_back_button_click));
    
    // В GTK4 кнопки добавляются в HeaderBar
    m_HeaderBar.pack_start(m_LR9BackButton);
    m_LR9BackButton.set_visible(false); 
    
    set_titlebar(m_HeaderBar);
}

void LRAppWindow::on_back_button_click() {
    m_Stack.set_visible_child("main_menu");
    set_title_bar("Главное меню");
    m_LR9BackButton.set_visible(false);
}

/**
 * @brief Имитирует запуск внешней программы labaN.
 */
std::string LRAppWindow::simulate_external_laba_execution(const std::string& input_source, const std::string& data) {
    std::string laba_name = m_current_laba_executable;

    std::string command_line;
    std::string input_details;

    if (input_source == "stdin") {
        command_line = "./" + laba_name + " < stdin_pipe (данные из GUI)";
        input_details = "Данные, введенные вручную:\n\n<tt>" + data + "</tt>\n";
    } else { // file path
        command_line = "./" + laba_name + " " + data;
        input_details = "Путь к входному файлу: <tt>" + data + "</tt>\n";
    }
    
    std::string result_text = "<span size='large' weight='bold'>" + m_current_lr_context + " - Выполнено</span>\n\n"
                         "--- Имитация запуска внешней программы ---\n"
                         "Предполагаемая команда для запуска:\n"
                         "<span foreground='#8A2BE2' weight='bold'>" + command_line + "</span>\n\n"
                         "--- Исходные данные ---\n" + input_details + "\n"
                         "--- Результат (Смоделирован) ---\n"
                         "Внешняя программа <span foreground='green' weight='bold'>" + laba_name + "</span> успешно обработала данные.\n"
                         "Результат вычислений: <span foreground='blue'>42.00</span>.\n"
                         "Для реального выполнения необходимо, чтобы программа <tt>" + laba_name + "</tt> находилась в той же директории и была исполняемой.";
    return result_text;
}

// --- Методы построения интерфейса ---

Gtk::Widget* LRAppWindow::build_lr_template_view(const std::string& title) {
    auto grid = Gtk::make_managed<Gtk::Grid>();
    grid->set_row_spacing(15);
    grid->set_column_spacing(15);
    grid->set_margin(40);
    grid->set_halign(Gtk::Align::CENTER);
    grid->set_valign(Gtk::Align::CENTER);

    auto title_label = Gtk::make_managed<Gtk::Label>(title);
    title_label->set_markup("<span size='large' weight='bold'>Выберите метод ввода</span>");

    auto manual_button = Gtk::make_managed<Gtk::Button>("1. Ввести значения вручную");
    manual_button->signal_clicked().connect(sigc::bind(
        sigc::mem_fun(*this, &LRAppWindow::on_input_mode_selected), "Вручную"));

    auto file_button = Gtk::make_managed<Gtk::Button>("2. Ввести значения из файла");
    file_button->signal_clicked().connect(sigc::bind(
        sigc::mem_fun(*this, &LRAppWindow::on_input_mode_selected), "Из файла"));
    
    manual_button->set_size_request(250, 50);
    file_button->set_size_request(250, 50);

    grid->attach(*title_label, 0, 0, 2, 1);
    grid->attach(*manual_button, 0, 1, 1, 1);
    grid->attach(*file_button, 1, 1, 1, 1);

    return grid;
}

Gtk::Widget* LRAppWindow::build_lr9_view() {
    auto grid = Gtk::make_managed<Gtk::Grid>();
    grid->set_row_spacing(10);
    grid->set_column_spacing(10);
    grid->set_margin(20);

    auto input_label = Gtk::make_managed<Gtk::Label>();
    input_label->set_markup("<b>Введите текст или загрузите файл:</b>");
    input_label->set_halign(Gtk::Align::START);
    grid->attach(*input_label, 0, 0, 2, 1);

    m_LR9InputText.get_buffer()->set_text("Вставьте сюда текст для анализа (ЛР9)!");
    auto scrolled_input = Gtk::make_managed<Gtk::ScrolledWindow>();
    scrolled_input->set_child(m_LR9InputText);
    scrolled_input->set_expand();
    grid->attach(*scrolled_input, 0, 1, 2, 1);

    m_LR9LoadFileButton.set_label("Загрузить текст из файла");
    m_LR9LoadFileButton.signal_clicked().connect(sigc::mem_fun(*this, &LRAppWindow::on_lr9_load_file_click));
    m_LR9LoadFileButton.set_hexpand(true);
    grid->attach(m_LR9LoadFileButton, 0, 2, 1, 1);
    
    m_LR9AnalyzeButton.set_label("Выполнить анализ");
    m_LR9AnalyzeButton.signal_clicked().connect(sigc::mem_fun(*this, &LRAppWindow::on_lr9_analyze_click));
    m_LR9AnalyzeButton.set_hexpand(true);
    m_LR9AnalyzeButton.get_style_context()->add_class("suggested-action");
    grid->attach(m_LR9AnalyzeButton, 1, 2, 1, 1);

    auto results_label_title = Gtk::make_managed<Gtk::Label>();
    results_label_title->set_markup("<b>Результаты анализа:</b>");
    results_label_title->set_halign(Gtk::Align::START);
    grid->attach(*results_label_title, 0, 3, 2, 1);

    m_LR9ResultsLabel.set_markup("<i>Результаты будут отображены здесь.</i>");
    m_LR9ResultsLabel.set_halign(Gtk::Align::START);
    m_LR9ResultsLabel.set_justify(Gtk::Justification::LEFT);
    m_LR9ResultsLabel.set_wrap(true);
    m_LR9ResultsLabel.set_expand();
    auto scrolled_results = Gtk::make_managed<Gtk::ScrolledWindow>();
    scrolled_results->set_child(m_LR9ResultsLabel);
    grid->attach(*scrolled_results, 0, 4, 2, 1);

    return grid;
}

Gtk::Widget* LRAppWindow::build_main_menu() {
    auto grid = Gtk::make_managed<Gtk::Grid>();
    grid->set_row_spacing(10);
    grid->set_column_spacing(10);
    grid->set_margin(30);
    grid->set_halign(Gtk::Align::CENTER);
    grid->set_valign(Gtk::Align::CENTER);

    auto title_label = Gtk::make_managed<Gtk::Label>();
    title_label->set_markup("<span size='xx-large' weight='bold'>Меню Лабораторных Работ</span>");
    grid->attach(*title_label, 0, 0, 4, 1);

    int row = 1;
    for (int lr = 1; lr <= 8; ++lr) {
        // Для ЛР 1, 3-8
        if (lr != 2) {
            auto button = Gtk::make_managed<Gtk::Button>("ЛР №" + std::to_string(lr));
            button->set_size_request(180, 40);
            button->signal_clicked().connect(sigc::bind(
                sigc::mem_fun(*this, &LRAppWindow::on_main_menu_button_click), lr, 0));
            grid->attach(*button, (lr - 1) % 4, row, 1, 1);
        }
        
        // Для ЛР 2: подменю с Заданиями 1 и 2
        if (lr == 2) {
            auto btn1 = Gtk::make_managed<Gtk::Button>("ЛР №2, Задание 1");
            btn1->set_size_request(180, 40);
            btn1->signal_clicked().connect(sigc::bind(
                sigc::mem_fun(*this, &LRAppWindow::on_main_menu_button_click), 2, 1));
            grid->attach(*btn1, (lr - 1) % 4, row, 1, 1);

            row++;
            
            auto btn2 = Gtk::make_managed<Gtk::Button>("ЛР №2, Задание 2");
            btn2->set_size_request(180, 40);
            btn2->signal_clicked().connect(sigc::bind(
                sigc::mem_fun(*this, &LRAppWindow::on_main_menu_button_click), 2, 2));
            grid->attach(*btn2, (lr - 1) % 4, row, 1, 1);
        }
        
        if (lr != 2 && lr % 4 == 0) {
            row++;
        }
    }
    
    // Индивидуальное задание (ЛР9)
    auto individual_button = Gtk::make_managed<Gtk::Button>("9. ИНДИВИДУАЛЬНОЕ ЗАДАНИЕ (Анализ Текста)");
    individual_button->get_style_context()->add_class("suggested-action");
    individual_button->set_size_request(250, 50);
    individual_button->signal_clicked().connect(sigc::mem_fun(*this, &LRAppWindow::on_lr9_analyze_click));

    row++;
    grid->attach(*individual_button, 0, row, 4, 1);
    
    return grid;
}

// --- Обработчики сигналов ---

void LRAppWindow::on_main_menu_button_click(int lr_number, int task_number) {
    if (lr_number == 9) {
        m_current_lr_context = "Индивидуальное Задание";
        m_current_laba_executable = get_laba_executable_name(lr_number, task_number);
        m_Stack.set_visible_child("lr9_view");
        set_title_bar("Индивидуальное Задание (Анализ Текста)");
    } else {
        std::string lr_context = "ЛР №" + std::to_string(lr_number);
        if (task_number > 0) {
            lr_context += ", Задание " + std::to_string(task_number);
        }
        m_current_lr_context = lr_context;
        m_current_laba_executable = get_laba_executable_name(lr_number, task_number);
        
        m_Stack.set_visible_child("lr_template");
        set_title_bar(lr_context);
    }
    m_LR9BackButton.set_visible(true);
}

void LRAppWindow::on_input_mode_selected(const std::string& mode) {
    if (m_current_lr_context.empty()) return;

    if (mode == "Вручную") {
        show_manual_input_dialog();
    } else { // Из файла
        // Запуск диалога выбора входного файла для labaN
        Gtk::FileChooserDialog* dialog = new Gtk::FileChooserDialog(
            "Выберите входной файл для " + m_current_laba_executable, Gtk::FileChooser::Action::OPEN);
        
        dialog->set_transient_for(*this);
        dialog->add_button("Отмена", Gtk::ResponseType::CANCEL);
        dialog->add_button("Запустить", Gtk::ResponseType::OK);

        auto filter_any = Gtk::FileFilter::create();
        filter_any->set_name("Все файлы");
        filter_any->add_pattern("*");
        dialog->add_filter(filter_any);

        // Привязываем асинхронный обработчик
        dialog->signal_response().connect(
            sigc::bind(sigc::mem_fun(*this, &LRAppWindow::on_file_chooser_response_lr_template), dialog)
        );
        
        dialog->present();
    }
}

// --- LR1-LR8 Dialog Handlers ---

/**
 * @brief Отображает диалог для ручного ввода данных.
 */
void LRAppWindow::show_manual_input_dialog() {
    auto dialog = Gtk::make_managed<Gtk::Dialog>("Ручной ввод данных для " + m_current_laba_executable, *this);
    dialog->set_default_size(400, 300);

    auto content_area = dialog->get_content_area();
    content_area->set_margin(10);
    
    auto label = Gtk::make_managed<Gtk::Label>("Введите данные, которые будут переданы в программу " + m_current_laba_executable + ":");
    label->set_halign(Gtk::Align::START);
    content_area->append(*label);

    auto text_view = Gtk::make_managed<Gtk::TextView>();
    text_view->get_buffer()->set_text("10\n20\n30.5"); // Пример данных
    auto scrolled_window = Gtk::make_managed<Gtk::ScrolledWindow>();
    scrolled_window->set_child(*text_view);
    scrolled_window->set_expand();
    content_area->append(*scrolled_window);

    dialog->add_button("Отмена", Gtk::ResponseType::CANCEL);
    dialog->add_button("Запустить", Gtk::ResponseType::OK);
    
    // Привязка асинхронного обработчика с передачей буфера текста
    dialog->signal_response().connect(
        sigc::bind(sigc::mem_fun(*this, &LRAppWindow::on_manual_input_dialog_response), dialog, text_view->get_buffer())
    );

    dialog->present();
}

/**
 * @brief Обработчик ответа от диалога ручного ввода.
 */
void LRAppWindow::on_manual_input_dialog_response(int response_id, Gtk::Dialog* dialog, Glib::RefPtr<Gtk::TextBuffer> buffer) {
    if (response_id == Gtk::ResponseType::OK) {
        std::string input_data = buffer->get_text();
        std::string result = simulate_external_laba_execution("stdin", input_data);
        display_results_dialog(m_current_lr_context + " - Имитация выполнения", result);
    }
    delete dialog;
}

/**
 * @brief Обработчик ответа от диалога выбора файла (для LR1-LR8).
 */
void LRAppWindow::on_file_chooser_response_lr_template(int response_id, Gtk::FileChooserDialog* dialog) {
    if (response_id == Gtk::ResponseType::OK) {
        auto file = dialog->get_file();
        if (file) {
            std::string filename = file->get_path();
            std::string result = simulate_external_laba_execution("file", filename);
            display_results_dialog(m_current_lr_context + " - Имитация выполнения", result);
        } else {
            display_results_dialog("Ошибка ввода", "<span foreground='red'>Ошибка: Файл не выбран.</span>");
        }
    }
    delete dialog;
}

/**
 * @brief Асинхронно отображает результаты выполнения ЛР в новом диалоговом окне.
 */
void LRAppWindow::display_results_dialog(const std::string& title, const std::string& result_text) {
    auto dialog = Gtk::make_managed<Gtk::MessageDialog>(*this, title, false, Gtk::MessageType::INFO, Gtk::ButtonsType::OK, true);
    
    auto label = Gtk::make_managed<Gtk::Label>();
    label->set_markup(result_text);
    label->set_halign(Gtk::Align::START);
    label->set_justify(Gtk::Justification::LEFT);
    label->set_wrap(true);
    
    auto content_area = dialog->get_content_area();
    content_area->append(*label);
    
    // После выполнения возвращаемся в Главное меню
    dialog->signal_response().connect(sigc::mem_fun(*this, &LRAppWindow::on_back_button_click));

    dialog->present();
}

// --- LR9 HANDLERS ---

void LRAppWindow::on_lr9_analyze_click() {
    // Переход на страницу LR9, если вызвана с Главного меню
    if (m_Stack.get_visible_child_name() != "lr9_view") {
        on_main_menu_button_click(9);
        return;
    }
    
    std::string text_to_analyze = m_LR9InputText.get_buffer()->get_text();
    TextAnalysisResult result = analyze_text(text_to_analyze);
    
    std::string output_markup;
    if (!result.error_message.empty()) {
        output_markup = "<span foreground='red' weight='bold'>Ошибка:</span> " + result.error_message;
    } else {
        std::ostringstream oss;
        oss << "<span weight='bold'>Общая статистика:</span>\n"
            << "  • Символов (включая пробелы): <span foreground='blue'>" << result.char_count << "</span>\n"
            << "  • Слов (всего): <span foreground='blue'>" << result.word_count << "</span>\n"
            << "  • Предложений (приблизительно): <span foreground='blue'>" << result.sentence_count << "</span>\n"
            << "  • Средняя длина слова (без стоп-слов): <span foreground='blue'>" << std::fixed << std::setprecision(2) << result.avg_word_length << "</span>\n\n"
            << "<span weight='bold'>Топ-10 самых частых слов (длина > 2):</span>\n";
        
        if (result.top_words.empty()) {
            oss << "  Нет слов, удовлетворяющих условию (длина > 2).";
        } else {
            int rank = 1;
            for (const auto& pair : result.top_words) {
                oss << "  " << rank++ << ". <span foreground='green'>" << pair.first << "</span> (Вхождений: " << pair.second << ")\n";
            }
        }
        output_markup = oss.str();
    }
    
    m_LR9ResultsLabel.set_markup(output_markup);
}

void LRAppWindow::on_lr9_load_file_click() {
    Gtk::FileChooserDialog* dialog = new Gtk::FileChooserDialog(
        "Выберите текстовый файл для анализа (ЛР9)", Gtk::FileChooser::Action::OPEN);
    
    dialog->set_transient_for(*this);
    dialog->add_button("Отмена", Gtk::ResponseType::CANCEL);
    dialog->add_button("Открыть", Gtk::ResponseType::OK);

    auto filter_text = Gtk::FileFilter::create();
    filter_text->set_name("Текстовые файлы (*.txt)");
    filter_text->add_mime_type("text/plain");
    dialog->add_filter(filter_text);
    
    auto filter_any = Gtk::FileFilter::create();
    filter_any->set_name("Все файлы");
    filter_any->add_pattern("*");
    dialog->add_filter(filter_any);

    dialog->signal_response().connect([this, dialog](int response_id) {
        if (response_id == Gtk::ResponseType::OK) {
            auto file = dialog->get_file();
            if (file) {
                std::string filename = file->get_path();
                std::ifstream fs(filename);
                if (fs.is_open()) {
                    std::string content((std::istreambuf_iterator<char>(fs)),
                                         std::istreambuf_iterator<char>());
                    m_LR9InputText.get_buffer()->set_text(content);
                    m_LR9ResultsLabel.set_markup("<span foreground='green'>Файл успешно загружен. Нажмите \"Выполнить анализ\".</span>");
                } else {
                    m_LR9ResultsLabel.set_markup("<span foreground='red'>Ошибка: Не удалось открыть файл:</span> " + filename);
                }
            } else {
                m_LR9ResultsLabel.set_markup("<span foreground='red'>Ошибка: Файл не выбран.</span>");
            }
        }
        delete dialog;
    });
    
    dialog->present();
}

// --- MAIN FUNCTION ---

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create("org.gemini.lrproject");

    return app->make_window_and_run<LRAppWindow>(argc, argv, app);
}
