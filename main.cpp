#include <gtk/gtk.h>
#include <fstream>
#include <iostream>
#include <gtk-3.0/gtk/gtk.h>

// Declare the text view and buffer for text editing
GtkWidget *textView;
GtkTextBuffer *textBuffer;

// Callback function to handle window close event
static gboolean on_delete_event(GtkWidget *widget, GdkEvent *event, gpointer data) {
    gtk_main_quit();
    return FALSE;
}

// Open file callback
static void on_open_file(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog;
    gchar *filename;
    GtkTextIter start, end;
    GtkTextBuffer *buffer = textBuffer;

    // Create a file chooser dialog
    dialog = gtk_file_chooser_dialog_new("Open File", GTK_WINDOW(gtk_widget_get_toplevel(widget)),
                                        GTK_FILE_CHOOSER_ACTION_OPEN, "_Cancel", GTK_RESPONSE_CANCEL,
                                        "_Open", GTK_RESPONSE_ACCEPT, NULL);

    // Run the dialog
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));

        // Open the file and load its content into the text buffer
        std::ifstream file(filename);
        if (file.is_open()) {
            std::string line;
            gtk_text_buffer_set_text(buffer, "", -1);  // Clear the text buffer

            while (std::getline(file, line)) {
                gtk_text_buffer_get_start_iter(buffer, &start);
                gtk_text_buffer_get_end_iter(buffer, &end);
                gtk_text_buffer_insert(buffer, &end, line.c_str(), line.length());
                gtk_text_buffer_insert(buffer, &end, "\n", 1);  // Add a new line
            }

            file.close();
        }

        g_free(filename);
    }
    gtk_widget_destroy(dialog);
}

// Save file callback
static void on_save_file(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog;
    gchar *filename;
    GtkTextIter start, end;
    GtkTextBuffer *buffer = textBuffer;

    // Create a file chooser dialog
    dialog = gtk_file_chooser_dialog_new("Save File", GTK_WINDOW(gtk_widget_get_toplevel(widget)),
                                        GTK_FILE_CHOOSER_ACTION_SAVE, "_Cancel", GTK_RESPONSE_CANCEL,
                                        "_Save", GTK_RESPONSE_ACCEPT, NULL);

    // Run the dialog
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));

        // Open the file and save the content from the text buffer
        std::ofstream file(filename);
        if (file.is_open()) {
            gtk_text_buffer_get_start_iter(buffer, &start);
            gtk_text_buffer_get_end_iter(buffer, &end);
            gchar *text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
            file << text;
            g_free(text);
            file.close();
        }

        g_free(filename);
    }
    gtk_widget_destroy(dialog);
}

// Initialize the editor window
void create_editor_window() {
    GtkWidget *window;
    GtkWidget *scrollWindow;
    GtkWidget *vbox;
    GtkWidget *menubar;
    GtkWidget *fileMenu;
    GtkWidget *fileMenuItem;
    GtkWidget *openItem;
    GtkWidget *saveItem;
    GtkWidget *exitItem;

    // Create main window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Lesedium Editor");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    g_signal_connect(window, "delete-event", G_CALLBACK(on_delete_event), NULL);

    // Create a vertical box to hold the menu and text view
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Create the menubar
    menubar = gtk_menu_bar_new();

    // Create the "File" menu
    fileMenu = gtk_menu_new();
    fileMenuItem = gtk_menu_item_new_with_label("File");
    openItem = gtk_menu_item_new_with_label("Open");
    saveItem = gtk_menu_item_new_with_label("Save");
    exitItem = gtk_menu_item_new_with_label("Exit");

    // Connect menu items to callbacks
    g_signal_connect(openItem, "activate", G_CALLBACK(on_open_file), NULL);
    g_signal_connect(saveItem, "activate", G_CALLBACK(on_save_file), NULL);
    g_signal_connect(exitItem, "activate", G_CALLBACK(on_delete_event), NULL);

    // Add menu items to the "File" menu
    gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), openItem);
    gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), saveItem);
    gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), exitItem);

    // Add the "File" menu to the menubar
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(fileMenuItem), fileMenu);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), fileMenuItem);

    // Create the text view for editing text
    textView = gtk_text_view_new();
    textBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView));

    // Add the menubar and text view to the vertical box
    gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), textView, TRUE, TRUE, 0);

    // Show all widgets
    gtk_widget_show_all(window);
}

// Main function to initialize GTK and run the editor
int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    create_editor_window();

    // Start the GTK main loop
    gtk_main();

    return 0;
}
