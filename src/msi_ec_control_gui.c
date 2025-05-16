#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "msi_ec_control_gui.h"

void write_to_file(const char *path, const char *value) {
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    fprintf(file, "%s", value);
    fclose(file);
    printf("Successfully wrote '%s' to %s\n", value, path);
}

char *read_from_file(const char *path) {
    char *buffer = malloc(256);
    if (buffer == NULL) {
        perror("Error allocating memory");
        return NULL;
    }
    
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        perror("Error opening file");
        free(buffer);
        return NULL;
    }
    
    if (fgets(buffer, 256, file) == NULL) {
        perror("Error reading file");
        fclose(file);
        free(buffer);
        return NULL;
    }
    
    fclose(file);
    buffer[strcspn(buffer, "\n")] = 0; // Remove trailing newline character
    return buffer;
}

void on_enable_webcam(GtkWidget *widget, gpointer data) {
    write_to_file("/sys/devices/platform/msi-ec/webcam", "on");
    on_refresh_status(widget, data);
}

void on_disable_webcam(GtkWidget *widget, gpointer data) {
    write_to_file("/sys/devices/platform/msi-ec/webcam", "off");
    on_refresh_status(widget, data);
}

void on_set_battery_mode(GtkWidget *widget, gpointer data) {
    const char *mode = (const char *)data;
    write_to_file("/sys/devices/platform/msi-ec/battery_mode", mode);
    on_refresh_status(widget, data);
}

void on_enable_cooler_boost(GtkWidget *widget, gpointer data) {
    write_to_file("/sys/devices/platform/msi-ec/cooler_boost", "on");
    on_refresh_status(widget, data);
}

void on_disable_cooler_boost(GtkWidget *widget, gpointer data) {
    write_to_file("/sys/devices/platform/msi-ec/cooler_boost", "off");
    on_refresh_status(widget, data);
}

void on_eco_mode(GtkWidget *widget, gpointer data) {
    write_to_file("/sys/devices/platform/msi-ec/shift_mode", "eco");
    on_refresh_status(widget, data);
}

void on_comfort_mode(GtkWidget *widget, gpointer data) {
    write_to_file("/sys/devices/platform/msi-ec/shift_mode", "comfort");
    on_refresh_status(widget, data);
}

void on_sport_mode(GtkWidget *widget, gpointer data) {
    write_to_file("/sys/devices/platform/msi-ec/shift_mode", "sport");
    on_refresh_status(widget, data);
}

void on_turbo_mode(GtkWidget *widget, gpointer data) {
    write_to_file("/sys/devices/platform/msi-ec/shift_mode", "turbo");
    on_refresh_status(widget, data);
}

void update_status_labels(GtkLabel *webcam_status, GtkLabel *cooler_boost_status, GtkLabel *battery_mode_status, GtkLabel *shift_mode_status) {
    char *webcam = read_from_file("/sys/devices/platform/msi-ec/webcam");
    char *cooler_boost = read_from_file("/sys/devices/platform/msi-ec/cooler_boost");
    char *battery_mode = read_from_file("/sys/devices/platform/msi-ec/battery_mode");
    char *shift_mode = read_from_file("/sys/devices/platform/msi-ec/shift_mode");

    if (webcam) {
        gtk_label_set_text(webcam_status, g_strdup_printf("Webcam: %s", webcam));
        free(webcam);
    }
    if (cooler_boost) {
        gtk_label_set_text(cooler_boost_status, g_strdup_printf("Cooler Boost: %s", cooler_boost));
        free(cooler_boost);
    }
    if (battery_mode) {
        gtk_label_set_text(battery_mode_status, g_strdup_printf("Battery Mode: %s", battery_mode));
        free(battery_mode);
    }
    if (shift_mode) {
        gtk_label_set_text(shift_mode_status, g_strdup_printf("Shift Mode: %s", shift_mode));
        free(shift_mode);
    }
} 

void on_refresh_status(GtkWidget *widget, gpointer data) {
    GtkLabel **labels = (GtkLabel **)data;
    update_status_labels(labels[0], labels[1], labels[2], labels[3]);
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *enable_webcam_button;
    GtkWidget *disable_webcam_button;
    GtkWidget *enable_cooler_boost_button;
    GtkWidget *disable_cooler_boost_button;
    GtkWidget *battery_mode_max_button;
    GtkWidget *battery_mode_medium_button;
    GtkWidget *battery_mode_min_button;
    GtkWidget *webcam_status_label;
    GtkWidget *cooler_boost_status_label;
    GtkWidget *battery_mode_status_label;
    GtkWidget *shift_mode_status_label;
    GtkWidget *eco_mode_button;
    GtkWidget *comfort_mode_button;
    GtkWidget *sport_mode_button;
    GtkWidget *turbo_mode_button;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "MSI EC Control Panel");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 600);

    grid = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_container_add(GTK_CONTAINER(window), grid);

    enable_webcam_button = gtk_button_new_with_label("Enable Webcam");
    disable_webcam_button = gtk_button_new_with_label("Disable Webcam");
    enable_cooler_boost_button = gtk_button_new_with_label("Enable Cooler Boost");
    disable_cooler_boost_button = gtk_button_new_with_label("Disable Cooler Boost");
    battery_mode_max_button = gtk_button_new_with_label("Battery Mode: Max");
    battery_mode_medium_button = gtk_button_new_with_label("Battery Mode: Medium");
    battery_mode_min_button = gtk_button_new_with_label("Battery Mode: Min");
    eco_mode_button = gtk_button_new_with_label("Eco Mode");
    comfort_mode_button = gtk_button_new_with_label("Comfort Mode");
    sport_mode_button = gtk_button_new_with_label("Sport Mode");
    turbo_mode_button = gtk_button_new_with_label("Turbo Mode");

    webcam_status_label = gtk_label_new("Webcam: Unknown");
    cooler_boost_status_label = gtk_label_new("Cooler Boost: Unknown");
    battery_mode_status_label = gtk_label_new("Battery Mode: Unknown");
    shift_mode_status_label = gtk_label_new("Shift Mode: Unknown");

    GtkWidget *separator1_label = gtk_label_new("📷Webcam Control");
    GtkWidget *separator2_label = gtk_label_new("🌬️Fan Control");
    GtkWidget *separator3_label = gtk_label_new("🔋Battery Control");
    GtkWidget *separator4_label = gtk_label_new("Shift Control");


    GtkLabel *status_labels[] = {
        GTK_LABEL(webcam_status_label),
        GTK_LABEL(cooler_boost_status_label),
        GTK_LABEL(battery_mode_status_label),
        GTK_LABEL(shift_mode_status_label)
    };

    g_signal_connect(enable_webcam_button, "clicked", G_CALLBACK(on_enable_webcam), status_labels);
    g_signal_connect(disable_webcam_button, "clicked", G_CALLBACK(on_disable_webcam), status_labels);
    g_signal_connect(enable_cooler_boost_button, "clicked", G_CALLBACK(on_enable_cooler_boost), status_labels);
    g_signal_connect(disable_cooler_boost_button, "clicked", G_CALLBACK(on_disable_cooler_boost), status_labels);
    g_signal_connect(battery_mode_max_button, "clicked", G_CALLBACK(on_set_battery_mode), (gpointer)"max");
    g_signal_connect(battery_mode_medium_button, "clicked", G_CALLBACK(on_set_battery_mode), (gpointer)"medium");
    g_signal_connect(battery_mode_min_button, "clicked", G_CALLBACK(on_set_battery_mode), (gpointer)"min");
    g_signal_connect(eco_mode_button, "clicked", G_CALLBACK(on_eco_mode), status_labels);
    g_signal_connect(comfort_mode_button, "clicked", G_CALLBACK(on_comfort_mode), status_labels);
    g_signal_connect(sport_mode_button, "clicked", G_CALLBACK(on_sport_mode), status_labels);
    g_signal_connect(turbo_mode_button, "clicked", G_CALLBACK(on_turbo_mode), status_labels);

    gtk_grid_attach(GTK_GRID(grid), separator1_label, 0, 0, 2, 1); // Add label above webcam control
    gtk_grid_attach(GTK_GRID(grid), enable_webcam_button, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), disable_webcam_button, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), separator2_label, 0, 2, 2, 1); // Add label above cooler boost control
    gtk_grid_attach(GTK_GRID(grid), enable_cooler_boost_button, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), disable_cooler_boost_button, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), separator3_label, 0, 4, 2, 1); // Add label above battery mode control
    gtk_grid_attach(GTK_GRID(grid), battery_mode_max_button, 0, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), battery_mode_medium_button, 1, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), battery_mode_min_button, 0, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), separator4_label, 0, 7, 2, 1); // Add label above shift mode control
    gtk_grid_attach(GTK_GRID(grid), eco_mode_button, 0, 8, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), comfort_mode_button, 1, 8, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), sport_mode_button, 0, 9, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), turbo_mode_button, 1, 9, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), webcam_status_label, 0, 10, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), cooler_boost_status_label, 0, 11, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), battery_mode_status_label, 0, 12, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), shift_mode_status_label, 0, 13, 2, 1);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
