#include <gtk/gtk.h>

void on_slider_changed(GtkRange *range, gpointer label) {
    char value[32];
    double slider_value = gtk_range_get_value(range);
    snprintf(value, sizeof(value), "Value: %.2f", slider_value);
    gtk_label_set_text(GTK_LABEL(label), value);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Slider Test");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), box);

    GtkWidget *slider = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0.0, 100.0, 1.0);
    gtk_box_pack_start(GTK_BOX(box), slider, FALSE, FALSE, 0);

    GtkWidget *label = gtk_label_new("Value: 0.00");
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);

    g_signal_connect(slider, "value-changed", G_CALLBACK(on_slider_changed), label);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
