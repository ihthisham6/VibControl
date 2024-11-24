#include "gui.h"
#include "vibration.h"
#include <gtk/gtk.h>

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data);
static void on_simulate_button_clicked(GtkButton *button, gpointer user_data);
static void on_export_button_clicked(GtkButton *button, gpointer user_data);

void start_gui() {
    GtkWidget *window, *notebook, *tab_simulation, *tab_export;
    GtkWidget *grid, *entry_frequency, *entry_amplitude, *entry_damping, *entry_time;
    GtkWidget *simulate_button, *export_button, *drawing_area;

    gtk_init(NULL, NULL);

    // Create main window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Advanced Vibration Control");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Notebook for tabs
    notebook = gtk_notebook_new();
    gtk_container_add(GTK_CONTAINER(window), notebook);

    // Simulation tab
    tab_simulation = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);

    // Input fields
    entry_frequency = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_frequency), "Frequency (Hz)");
    entry_amplitude = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_amplitude), "Amplitude");
    entry_damping = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_damping), "Damping");
    entry_time = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_time), "Max Time (s)");

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Frequency:"), 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_frequency, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Amplitude:"), 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_amplitude, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Damping:"), 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_damping, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Max Time:"), 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_time, 1, 3, 1, 1);

    gtk_box_pack_start(GTK_BOX(tab_simulation), grid, FALSE, FALSE, 0);

    simulate_button = gtk_button_new_with_label("Simulate");
    gtk_box_pack_start(GTK_BOX(tab_simulation), simulate_button, FALSE, FALSE, 0);

    drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawing_area, 500, 300);
    gtk_box_pack_start(GTK_BOX(tab_simulation), drawing_area, TRUE, TRUE, 0);

    GtkWidget *inputs[5] = {entry_frequency, entry_amplitude, entry_damping, entry_time, drawing_area};
    g_signal_connect(simulate_button, "clicked", G_CALLBACK(on_simulate_button_clicked), inputs);
    g_signal_connect(drawing_area, "draw", G_CALLBACK(on_draw_event), NULL);

    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), tab_simulation, gtk_label_new("Simulation"));

    // Export tab
    tab_export = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    export_button = gtk_button_new_with_label("Export Data to CSV");
    gtk_box_pack_start(GTK_BOX(tab_export), export_button, FALSE, FALSE, 0);
    g_signal_connect(export_button, "clicked", G_CALLBACK(on_export_button_clicked), NULL);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), tab_export, gtk_label_new("Export"));

    gtk_widget_show_all(window);
    gtk_main();
}

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
    if (data_count == 0) return FALSE;

    // Draw axes and data (code unchanged for brevity)
    return FALSE;
}

static void on_simulate_button_clicked(GtkButton *button, gpointer user_data) {
    // Get input values and call simulation (code unchanged for brevity)
}

static void on_export_button_clicked(GtkButton *button, gpointer user_data) {
    export_to_csv("vibration_data.csv");
}
