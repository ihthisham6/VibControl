#include <gtk/gtk.h>
#include <cairo.h>
#include <math.h>
#include <stdlib.h>

// Global variables for simulation
double *time_values = NULL, *displacement = NULL;
int data_count = 0;
double frequency = 1.0, amplitude = 1.0, damping = 0.1, max_time = 10.0;

// Simulate vibration data
void simulate_vibration() {
    free(time_values);
    free(displacement);

    double time_step = 0.01;
    data_count = (int)(max_time / time_step) + 1;
    time_values = malloc(data_count * sizeof(double));
    displacement = malloc(data_count * sizeof(double));

    for (int i = 0; i < data_count; i++) {
        time_values[i] = i * time_step;
        displacement[i] = amplitude * exp(-damping * time_values[i]) * sin(2 * M_PI * frequency * time_values[i]);
    }
}

// Draw the plot
void on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
    GtkAllocation allocation;
    gtk_widget_get_allocation(widget, &allocation);
    double width = allocation.width, height = allocation.height;

    cairo_set_source_rgb(cr, 1, 1, 1); // Clear background
    cairo_paint(cr);

    cairo_set_source_rgb(cr, 0, 0, 0); // Draw axes
    cairo_move_to(cr, 0, height / 2);
    cairo_line_to(cr, width, height / 2);
    cairo_stroke();

    if (data_count > 1) {
        cairo_set_source_rgb(cr, 0, 0, 1); // Draw vibration data
        cairo_move_to(cr, 10, height / 2 - displacement[0] * height / 2);
        for (int i = 1; i < data_count; i++) {
            double x = 10 + i * (width - 20) / data_count;
            double y = height / 2 - displacement[i] * height / 2;
            cairo_line_to(cr, x, y);
        }
        cairo_stroke();
    }
}

// Update parameters when sliders are adjusted
void on_slider_changed(GtkRange *range, gpointer user_data) {
    GtkWidget *drawing_area = GTK_WIDGET(user_data);
    frequency = gtk_range_get_value(GTK_RANGE(range));
    simulate_vibration();
    gtk_widget_queue_draw(drawing_area);
}

// Main function
int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Vibration Simulator");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *paned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_container_add(GTK_CONTAINER(window), paned);

    // Left panel: Controls
    GtkWidget *control_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_size_request(control_box, 200, -1);
    gtk_paned_pack1(GTK_PANED(paned), control_box, FALSE, FALSE);

    GtkWidget *slider_frequency = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0.1, 10.0, 0.1);
    gtk_range_set_value(GTK_RANGE(slider_frequency), frequency);
    gtk_box_pack_start(GTK_BOX(control_box), gtk_label_new("Frequency"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(control_box), slider_frequency, FALSE, FALSE, 0);

    GtkWidget *slider_amplitude = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0.1, 10.0, 0.1);
    gtk_range_set_value(GTK_RANGE(slider_amplitude), amplitude);
    gtk_box_pack_start(GTK_BOX(control_box), gtk_label_new("Amplitude"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(control_box), slider_amplitude, FALSE, FALSE, 0);

    GtkWidget *slider_damping = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0.0, 1.0, 0.01);
    gtk_range_set_value(GTK_RANGE(slider_damping), damping);
    gtk_box_pack_start(GTK_BOX(control_box), gtk_label_new("Damping"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(control_box), slider_damping, FALSE, FALSE, 0);

    GtkWidget *slider_time = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 1.0, 30.0, 0.5);
    gtk_range_set_value(GTK_RANGE(slider_time), max_time);
    gtk_box_pack_start(GTK_BOX(control_box), gtk_label_new("Simulation Time"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(control_box), slider_time, FALSE, FALSE, 0);

    // Right panel: Plot area
    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_paned_pack2(GTK_PANED(paned), drawing_area, TRUE, FALSE);
    g_signal_connect(drawing_area, "draw", G_CALLBACK(on_draw_event), NULL);

    g_signal_connect(slider_frequency, "value-changed", G_CALLBACK(on_slider_changed), drawing_area);

    gtk_widget_show_all(window);

    simulate_vibration();
    gtk_main();

    free(time_values);
    free(displacement);
    return 0;
}
