#include <3ds.h>

#include "action.h"
#include "../../error.h"
#include "../../progressbar.h"
#include "../../prompt.h"

static void action_delete_pending_title_success_onresponse(ui_view* view, void* data, bool response) {
    prompt_destroy(view);
}

static void action_delete_pending_title_update(ui_view* view, void* data, float* progress, char* progressText) {
    pending_title_info* info = (pending_title_info*) data;

    Result res = AM_DeletePendingTitle(info->mediaType, info->titleId);
    if(R_FAILED(res)) {
        progressbar_destroy(view);
        ui_pop();

        error_display_res(info, ui_draw_pending_title_info, res, "Failed to delete pending title.");

        return;
    }

    progressbar_destroy(view);
    ui_pop();

    ui_push(prompt_create("Success", "Pending title deleted.", 0xFF000000, false, info, NULL, ui_draw_pending_title_info, action_delete_pending_title_success_onresponse));
}

static void action_delete_pending_title_onresponse(ui_view* view, void* data, bool response) {
    prompt_destroy(view);

    if(response) {
        ui_push(progressbar_create("Deleting Pending Title", "", data, action_delete_pending_title_update, ui_draw_pending_title_info));
    }
}

void action_delete_pending_title(pending_title_info* info) {
    ui_push(prompt_create("Confirmation", "Delete the selected pending title?", 0xFF000000, true, info, NULL, ui_draw_pending_title_info, action_delete_pending_title_onresponse));
}