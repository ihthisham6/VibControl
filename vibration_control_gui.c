#include <gtk/gtk.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <cairo.h>


#define MAX_POINTS 1000
void import_from_csv(GtkWidget *drawing_area);
// Global variables for storing data
double time_data[MAX_POINTS], displacement[MAX_POINTS];
int data_count = 0;

// Simulation parameters
double frequency = 1.0, amplitude = 1.0, damping = 0.1, max_time = 10.0;
int simulation_running = 0;

// Function prototypes
void simulate_vibration();
void redraw_plot(GtkWidget *widget);
void export_to_csv();
void start_simulation(GtkWidget *drawing_area);
void stop_simulation(GtkWidget *drawing_area);
void restart_simulation(GtkWidget *drawing_area, GtkWidget *freq_slider, GtkWidget *amp_slider, GtkWidget *damp_slider, GtkWidget *time_slider);


typedef struct {
    GtkWidget *drawing_area;
    GtkWidget *freq_slider;
    GtkWidget *amp_slider;
    GtkWidget *damp_slider;
    GtkWidget *time_slider;
} Widgets;
void restart_button_clicked(GtkWidget *button, gpointer user_data) {
    Widgets *widgets = (Widgets *)user_data;
    restart_simulation(widgets->drawing_area, widgets->freq_slider, widgets->amp_slider, widgets->damp_slider, widgets->time_slider);
}
void import_from_csv(GtkWidget *drawing_area) {
    // Open a file chooser dialog
    GtkWidget *dialog = gtk_file_chooser_dialog_new(
        "Open CSV File",
        NULL,
        GTK_FILE_CHOOSER_ACTION_OPEN,
        "_Cancel", GTK_RESPONSE_CANCEL,
        "_Open", GTK_RESPONSE_ACCEPT,
        NULL);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        FILE *file = fopen(filename, "r");
        if (!file) {
            fprintf(stderr, "Error opening file: %s\n", filename);
            g_free(filename);
            gtk_widget_destroy(dialog);
            return;
        }

        // Clear existing data
        data_count = 0;

        // Read the CSV file
        char line[256];
        fgets(line, sizeof(line), file); // Skip the header
        while (fgets(line, sizeof(line), file) && data_count < MAX_POINTS) {
            // Validate the line format (expecting two float values)
            if (sscanf(line, "%lf,%lf", &time_data[data_count], &displacement[data_count]) == 2) {
                data_count++;
            } else {
                fprintf(stderr, "Invalid line format in CSV: %s\n", line);
            }
        }

        fclose(file);
        g_free(filename);

        // Redraw the plot
        redraw_plot(drawing_area);
        printf("Data imported from CSV.\n");
    } else {
        printf("File import canceled.\n");
    }

    gtk_widget_destroy(dialog);
}



void apply_css() {
    GtkCssProvider *css_provider = gtk_css_provider_new();
    GdkDisplay *display = gdk_display_get_default();
    GdkScreen *screen = gdk_display_get_default_screen(display);

    // Load the CSS file
    gtk_css_provider_load_from_path(css_provider, "style.css", NULL);

    // Apply the style provider globally
    gtk_style_context_add_provider_for_screen(
        screen,
        GTK_STYLE_PROVIDER(css_provider),
        GTK_STYLE_PROVIDER_PRIORITY_USER
    );

    // Unreference CSS provider after use
    g_object_unref(css_provider);
}



// Function to simulate vibration
void simulate_vibration() {
    if (frequency <= 0 || amplitude <= 0 || max_time <= 0) {
        fprintf(stderr, "Invalid simulation parameters. Using defaults.\n");
        return; // Exit if parameters are invalid
    }

    double omega = 2 * M_PI * frequency;
    double time_step = 0.01, time_value = 0.0;
    data_count = 0;

    while (time_value <= max_time && data_count < MAX_POINTS) {
        displacement[data_count] = amplitude * exp(-damping * time_value) * cos(omega * time_value);
        time_data[data_count] = time_value;
        time_value += time_step;
        data_count++;
    }

    printf("Simulation data generated with frequency=%.2f, amplitude=%.2f, damping=%.2f, max_time=%.2f.\n",
           frequency, amplitude, damping, max_time);
}

// Function to redraw the plot
void redraw_plot(GtkWidget *widget) {
    if (widget) {
        gtk_widget_queue_draw(widget);
        printf("Plot redrawn.\n");
    }
}


// Function to export data to CSV
void export_to_csv() {
    FILE *file = fopen("vibration_data.csv", "w");
    if (!file) {
        fprintf(stderr, "Error opening file for export\n");
        return;
    }

    fprintf(file, "Time,Displacement\n");
    for (int i = 0; i < data_count; i++) {
        fprintf(file, "%.3f,%.3f\n", time_data[i], displacement[i]);
    }

    fclose(file);
    printf("Data exported to 'vibration_data.csv'\n");
}

// Callback: Start button clicked
// Callback: Start button clicked
void start_simulation(GtkWidget *drawing_area) {
    if (!simulation_running) {
        simulation_running = 1;
        simulate_vibration();
        redraw_plot(drawing_area);
        printf("Simulation started.\n");
    }
}

// Callback: Stop button clicked
void stop_simulation(GtkWidget *drawing_area) {
    simulation_running = 0;
}

// Callback: Restart button clicked
// Callback: Restart button clicked
// Callback: Restart button clicked
void restart_simulation(GtkWidget *drawing_area, GtkWidget *freq_slider, GtkWidget *amp_slider, GtkWidget *damp_slider, GtkWidget *time_slider) {
    // Reset parameters to default values
    frequency = 1.0;
    amplitude = 1.0;
    damping = 0.1;
    max_time = 10.0;

    // Clear data arrays
    data_count = 0;
    for (int i = 0; i < MAX_POINTS; i++) {
        time_data[i] = 0.0;
        displacement[i] = 0.0;
    }

    // Update slider positions to reflect defaults
    gtk_range_set_value(GTK_RANGE(freq_slider), frequency);
    gtk_range_set_value(GTK_RANGE(amp_slider), amplitude);
    gtk_range_set_value(GTK_RANGE(damp_slider), damping);
    gtk_range_set_value(GTK_RANGE(time_slider), max_time);

    // Generate new simulation data
    simulate_vibration();

    // Redraw the plot
    redraw_plot(drawing_area);

    printf("Simulation restarted and sliders reset.\n");
}




// Callback: Draw event
gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
    if (data_count == 0) return FALSE;

    // Draw axes
    cairo_set_line_width(cr, 1);
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_move_to(cr, 50, 250); cairo_line_to(cr, 400, 250); // X-axis
    cairo_move_to(cr, 50, 50); cairo_line_to(cr, 50, 250);  // Y-axis
    cairo_stroke(cr);

    // Plot data
    cairo_set_source_rgb(cr, 1, 0, 0);
    cairo_move_to(cr, 50 + time_data[0] * 30, 250 - displacement[0] * 100);
    for (int i = 1; i < data_count; i++) {
        cairo_line_to(cr, 50 + time_data[i] * 30, 250 - displacement[i] * 100);
    }
    cairo_stroke(cr);

    return FALSE;
}


// Callback: Slider changed
// Callback: Slider changed
void on_slider_changed(GtkRange *range, gpointer user_data) {
    const gchar *param = (const gchar *)user_data;

    // Update the corresponding variable based on the slider
    if (g_strcmp0(param, "frequency") == 0) {
        frequency = gtk_range_get_value(range);
    } else if (g_strcmp0(param, "amplitude") == 0) {
        amplitude = gtk_range_get_value(range);
    } else if (g_strcmp0(param, "damping") == 0) {
        damping = gtk_range_get_value(range);
    } else if (g_strcmp0(param, "max_time") == 0) {
        max_time = gtk_range_get_value(range);
    }

    // Generate new simulation data based on updated slider values
    simulate_vibration();

    // Redraw the plot
    GtkWidget *drawing_area = GTK_WIDGET(g_object_get_data(G_OBJECT(range), "drawing_area"));
    if (drawing_area) {
        gtk_widget_queue_draw(drawing_area);  // Forces the `on_draw_event` to be called
    }
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
 apply_css();

    // Main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Vibration Control");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Layout
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Drawing area
    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawing_area, 600, 300);
    gtk_grid_attach(GTK_GRID(grid), drawing_area, 0, 0, 4, 1);
    g_signal_connect(drawing_area, "draw", G_CALLBACK(on_draw_event), NULL);

    // Sliders
    GtkWidget *freq_slider = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0.1, 5.0, 0.1);
    gtk_range_set_value(GTK_RANGE(freq_slider), frequency);
    GtkWidget *amp_slider = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0.1, 5.0, 0.1);
    gtk_range_set_value(GTK_RANGE(amp_slider), amplitude);
    GtkWidget *damp_slider = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0.01, 1.0, 0.01);
    gtk_range_set_value(GTK_RANGE(damp_slider), damping);
    GtkWidget *time_slider = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 1.0, 20.0, 1.0);
    gtk_range_set_value(GTK_RANGE(time_slider), max_time);

    // Connect slider signals to the callback
    g_signal_connect(freq_slider, "value-changed", G_CALLBACK(on_slider_changed), (gpointer)"frequency");
    g_signal_connect(amp_slider, "value-changed", G_CALLBACK(on_slider_changed), (gpointer)"amplitude");
    g_signal_connect(damp_slider, "value-changed", G_CALLBACK(on_slider_changed), (gpointer)"damping");
    g_signal_connect(time_slider, "value-changed", G_CALLBACK(on_slider_changed), (gpointer)"max_time");

    // Add sliders to layout
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Frequency"), 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), freq_slider, 1, 1, 3, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Amplitude"), 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), amp_slider, 1, 2, 3, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Damping"), 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), damp_slider, 1, 3, 3, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Max Time"), 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), time_slider, 1, 4, 3, 1);

    // Buttons
    GtkWidget *start_button = gtk_button_new_with_label("Start");
    g_signal_connect(start_button, "clicked", G_CALLBACK(start_simulation), drawing_area);
    GtkWidget *stop_button = gtk_button_new_with_label("Stop");
    g_signal_connect(stop_button, "clicked", G_CALLBACK(stop_simulation), drawing_area);
    GtkWidget *restart_button = gtk_button_new_with_label("Restart");
    GtkWidget *export_button = gtk_button_new_with_label("Export to CSV");
    g_signal_connect(export_button, "clicked", G_CALLBACK(export_to_csv), NULL);
    GtkWidget *import_button = gtk_button_new_with_label("Import from CSV");
    g_signal_connect(import_button, "clicked", G_CALLBACK(import_from_csv), drawing_area);

    // Add buttons to layout
   
    gtk_grid_attach(GTK_GRID(grid), import_button, 4, 5, 1, 1);


    // Add buttons to layout
    gtk_grid_attach(GTK_GRID(grid), start_button, 0, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), stop_button, 1, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), restart_button, 2, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), export_button, 3, 5, 1, 1);




// Inside main()
g_object_set_data(G_OBJECT(freq_slider), "drawing_area", drawing_area);
g_object_set_data(G_OBJECT(amp_slider), "drawing_area", drawing_area);
g_object_set_data(G_OBJECT(damp_slider), "drawing_area", drawing_area);
g_object_set_data(G_OBJECT(time_slider), "drawing_area", drawing_area);



    // Create the widgets structure
    Widgets *widgets = g_malloc(sizeof(Widgets));
    widgets->drawing_area = drawing_area;
    widgets->freq_slider = freq_slider;
    widgets->amp_slider = amp_slider;
    widgets->damp_slider = damp_slider;
    widgets->time_slider = time_slider;

    // Connect the restart button with proper callback
    g_signal_connect(restart_button, "clicked", G_CALLBACK(restart_button_clicked), widgets);

    // Show all widgets
    gtk_widget_show_all(window);

    // Initial vibration simulation
    simulate_vibration();

    // Enter the GTK main loop
    gtk_main();

    // Free allocated memory
    g_free(widgets);

    return 0;
}
