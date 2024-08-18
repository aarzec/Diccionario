#include <gtkmm.h>
#include <gst/gst.h>
#include <gobject/gsignal.h>
#include <curl/curl.h>
#include <iostream>
#include <sstream>
#include "./lib/rapidjson/document.h"
#include "./lib/rapidjson/writer.h"
#include "./lib/rapidjson/stringbuffer.h"
#include "./utils/EnvReader.h"

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string buscarImagenPexels(const std::string& query, const std::string& apiKey) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "[pexels] Error al inicializar cURL" << std::endl;
        return "";
    }

    // Datos
    char* encodedQuery = curl_easy_escape(curl, query.c_str(), query.size());
    std::string url = "https://api.pexels.com/v1/search?query=" + std::string(encodedQuery) + "&per_page=1";
    curl_free(encodedQuery);
    
    // Headers
    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, ("Authorization: " + apiKey).c_str());

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    std::string responseString;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseString);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "[pexels] cURL Error: " << curl_easy_strerror(res) << std::endl;
        return "";
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    // Parsear JSON
    rapidjson::Document document;
    document.Parse(responseString.c_str());
    if (document.HasParseError()) {
        std::cerr << "[pexels] Error al parsear JSON: " << document.GetParseError() << ", JSON: " << responseString << std::endl;
        return "";
    }

    if (document["photos"].Size() == 0) {
        return "";
    }

    return document["photos"][0]["src"]["medium"].GetString();
}

void descargarImagen(const std::string& url, const std::string& filename) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Error al inicializar cURL" << std::endl;
        return;
    }

    FILE* file = fopen(filename.c_str(), "wb");
    if (!file) {
        std::cerr << "Error al abrir el archivo" << std::endl;
        return;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "[img] cURL Error: " << curl_easy_strerror(res) << std::endl;
    }

    fclose(file);
    curl_easy_cleanup(curl);
}

void eliminarArchivo(const std::string& filename) {
    if (remove(filename.c_str()) != 0) {
        std::cerr << "Error al eliminar el archivo" << std::endl;
    }
}

std::string traducir(const std::string& texto, const std::string& langOrigen, const std::string& langDestino, const std::string& apiKey) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        return "";
    }

    // Datos
    std::string url = "https://api-free.deepl.com/v2/translate";
    std::string authKey = "DeepL-Auth-Key " + apiKey;
    std::string jsonData = R"({
        "text": [
            ")" + texto + R"("
        ],
        "source_lang": ")" + langOrigen + R"(",
        "target_lang": ")" + langDestino + R"("
    })";
    std::string responseString;

    // Headers
    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, ("Authorization: " + authKey).c_str());
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseString);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "cURL Error: " << curl_easy_strerror(res) << std::endl;
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    // Parsear JSON
    rapidjson::Document document;
    document.Parse(responseString.c_str());
    if (document.HasParseError()) {
        std::cerr << "Error al parsear JSON" << std::endl;
        return "";
    }

    return document["translations"][0]["text"].GetString();
}

class MainWindow : public Gtk::Window {
    protected:
    Gtk::Box m_box;

    Gtk::Label m_label_origen;
    Gtk::Label m_label_destino;
    Gtk::ComboBoxText m_combo_origen;
    Gtk::ComboBoxText m_combo_destino;

    Gtk::Label m_label;
    Gtk::Entry m_entry;
    Gtk::Button m_button;
    Gtk::Label m_translated_label;

    Gtk::Button m_button_tts;

    Gtk::Image m_image;

    GstElement *pipeline;

    void on_button_clicked() {
        std::string input_text = m_entry.get_text();
        if (input_text.empty()) {
            m_translated_label.set_text("Por favor, introduce un texto.");
            return;
        }

        EnvReader env(".env");

        // Texto
        std::string deeplApiKey = env.get("DEEPL_API_KEY", "");
        std::string translated_text = traducir(input_text, m_combo_origen.get_active_id(), m_combo_destino.get_active_id(), deeplApiKey);
        m_translated_label.set_text(translated_text);

        // Imagen
        std::string pexelsApiKey = env.get("PEXELS_API_KEY", "");
        std::string query = translated_text;
        std::string imageUrl = buscarImagenPexels(query, pexelsApiKey);
        if (!imageUrl.empty()) {
            std::string filename = "image.jpg";
            descargarImagen(imageUrl, filename);
            m_image.set(filename);
            eliminarArchivo(filename);
        } else {
            m_image.set("blank.png");
        }

        // Audio
        // gst_init(nullptr, nullptr);

        // pipeline = gst_pipeline_new("audio-player");
        // GstElement *source = gst_element_factory_make("filesrc", "file-source");
        // GstElement *decoder = gst_element_factory_make("decodebin", "decoder");
        // GstElement *conv = gst_element_factory_make("audioconvert", "converter");
        // GstElement *sink = gst_element_factory_make("autoaudiosink", "audio-output");

        // if (!pipeline || !source || !decoder || !conv || !sink) {
        //     g_printerr("Not all elements could be created.\n");
        //     return;
        // }

        // g_object_set(G_OBJECT(source), "location", "sample.mp3", NULL);

        // gst_bin_add_many(GST_BIN(pipeline), source, decoder, conv, sink, NULL);
        // gst_element_link(source, decoder);
        // gst_element_link(conv, sink);

        // g_signal_connect(decoder, "pad-added", G_CALLBACK(on_pad_added), conv);
        // gst_element_set_state(pipeline, GST_STATE_PLAYING);
    }

    void on_button_tts_clicked() {
        std::string input_text = m_translated_label.get_text();
        if (input_text.empty()) {
            return;
        }
        std::string lang = m_combo_destino.get_active_id() == "ES" ? "es" : "en";

        system(("espeak -v " + lang + " \"" + input_text + "\"").c_str());
    }

    public:
    MainWindow() {
        set_title("Diccionario traductor");
        set_default_size(400, 200);

        m_box.set_orientation(Gtk::ORIENTATION_VERTICAL);
        add(m_box);

        // Labels
        m_label_origen.set_text("Origen:");
        m_box.pack_start(m_label_origen);
        m_combo_origen.append("ES", "Español");
        m_combo_origen.append("EN", "Inglés");
        m_combo_origen.set_active(0);
        m_box.pack_start(m_combo_origen);

        m_label_destino.set_text("Destino:");
        m_box.pack_start(m_label_destino);
        m_combo_destino.append("ES", "Español");
        m_combo_destino.append("EN", "Inglés");
        m_combo_destino.set_active(1);
        m_box.pack_start(m_combo_destino);

        // Entrada de texto
        m_label.set_text("Introduce el texto a traducir:");
        m_box.pack_start(m_label);
        m_box.pack_start(m_entry);

        // Botón traducir
        m_button.set_label("Traducir");
        m_button.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_button_clicked));
        m_box.pack_start(m_button);

        // Texto traducido
        m_box.pack_start(m_translated_label);

        // Botón TTS
        m_button_tts.set_label("Reproducir audio");
        m_button_tts.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_button_tts_clicked));
        m_box.pack_start(m_button_tts);

        // Imagen
        m_image.set("blank.png");
        m_box.pack_start(m_image);

        show_all_children();
    }

    static void on_pad_added(GstElement *src, GstPad *new_pad, GstElement *conv) {
        GstPad *sink_pad = gst_element_get_static_pad(conv, "sink");
        if (gst_pad_is_linked(sink_pad)) {
            g_object_unref(sink_pad);
            return;
        }

        GstCaps *new_pad_caps = gst_pad_get_current_caps(new_pad);
        GstStructure *new_pad_struct = gst_caps_get_structure(new_pad_caps, 0);
        const gchar *new_pad_type = gst_structure_get_name(new_pad_struct);

        if (!g_str_has_prefix(new_pad_type, "audio/x-raw")) {
            gst_caps_unref(new_pad_caps);
            g_object_unref(sink_pad);
            return;
        }

        gst_pad_link(new_pad, sink_pad);
        gst_caps_unref(new_pad_caps);
        g_object_unref(sink_pad);
    }

    ~MainWindow() {
        gst_element_set_state(pipeline, GST_STATE_NULL);
        gst_object_unref(pipeline);
    }
};

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

    MainWindow window;

    return app->run(window);
}
