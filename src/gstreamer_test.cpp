#include <gst/gst.h>
#include <glib.h>
#include <stdio.h> // For g_print

// Structure to hold all our GStreamer data
typedef struct _CustomData {
  GstElement *pipeline;
  GstElement *source;
  GstElement *sink;
  GMainLoop *loop;
} CustomData;

// Handler for the pad-added signal
static void pad_added_handler(GstElement *src, GstPad *new_pad, CustomData *data);

// Bus message handler
static gboolean bus_call(GstBus *bus, GstMessage *msg, CustomData *data) {
  GMainLoop *loop = data->loop;

  switch (GST_MESSAGE_TYPE(msg)) {
    case GST_MESSAGE_EOS:
      g_print("End-of-stream reached.\n");
      g_main_loop_quit(loop);
      break;
    case GST_MESSAGE_ERROR: {
      gchar *debug_info;
      GError *err;

      gst_message_parse_error(msg, &err, &debug_info);
      g_printerr("Error received from element %s: %s\n", GST_OBJECT_NAME(msg->src), err->message);
      g_printerr("Debugging information: %s\n", debug_info ? debug_info : "none");
      g_clear_error(&err);
      g_free(debug_info);
      g_main_loop_quit(loop);
      break;
    }
    case GST_MESSAGE_WARNING: {
      gchar *debug_info;
      GError *err;

      gst_message_parse_warning(msg, &err, &debug_info);
      g_printerr("Warning received from element %s: %s\n", GST_OBJECT_NAME(msg->src), err->message);
      g_printerr("Debugging information: %s\n", debug_info ? debug_info : "none");
      g_clear_error(&err);
      g_free(debug_info);
      break;
    }
    default:
      break;
  }
  return TRUE;
}

int main(int argc, char *argv[]) {
  CustomData data;
  GstBus *bus;
  GstStateChangeReturn ret;

  // Initialize GStreamer
  gst_init(&argc, &argv);

  // Initialize CustomData structure
  memset(&data, 0, sizeof(data));

  // Create the elements
  // videotestsrc: Generates a test video pattern
  data.source = gst_element_factory_make("videotestsrc", "source");
  // autovideosink: Automatically selects and configures a video sink
  data.sink = gst_element_factory_make("autovideosink", "sink");

  // Create the empty pipeline
  data.pipeline = gst_pipeline_new("test-pipeline");

  if (!data.pipeline || !data.source || !data.sink) {
    g_printerr("Not all elements could be created.\n");
    return -1;
  }

  // Build the pipeline
  // A GStreamer pipeline is a GstBin, so elements are added to it.
  gst_bin_add_many(GST_BIN(data.pipeline), data.source, data.sink, NULL);

  // Link the elements
  // videotestsrc -> autovideosink
  if (gst_element_link(data.source, data.sink) != TRUE) {
    g_printerr("Elements could not be linked.\n");
    gst_object_unref(data.pipeline);
    return -1;
  }

  // Modify the source's properties
  g_object_set(data.source, "pattern", 0, NULL); // 0 for smpte bars

  // Create a GLib Main Loop and set up a bus watch
  data.loop = g_main_loop_new(NULL, FALSE);
  bus = gst_element_get_bus(data.pipeline);
  gst_bus_add_watch(bus, (GstBusFunc)bus_call, &data);
  gst_object_unref(bus);

  // Set the pipeline to the playing state
  g_print("Setting pipeline to PLAYING...\n");
  ret = gst_element_set_state(data.pipeline, GST_STATE_PLAYING);
  if (ret == GST_STATE_CHANGE_FAILURE) {
    g_printerr("Unable to set the pipeline to the playing state.\n");
    gst_object_unref(data.pipeline);
    return -1;
  } else if (ret == GST_STATE_CHANGE_NO_PREROLL) {
    g_print("Pipeline is live and does not preroll, playing state will be reached asynchronously.\n");
  }


  // Start the GLib main loop. We will receive messages from the bus.
  g_print("Running...\n");
  g_main_loop_run(data.loop);

  // Main loop has quit, free resources
  g_print("Returned, stopping playback\n");
  gst_element_set_state(data.pipeline, GST_STATE_NULL);

  g_print("Deleting pipeline\n");
  gst_object_unref(data.pipeline); // This will also unref its elements
  g_main_loop_unref(data.loop);

  return 0;
}
