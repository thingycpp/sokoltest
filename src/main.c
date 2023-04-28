#define SOKOL_IMPL
#define SOKOL_GLCORE33
#include "sokol_gfx.h"
#include "sokol_log.h"
#include "sokol_glue.h"
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "cimgui.h"
#include "sokol_imgui.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

int main() {

    /* create window and GL context via GLFW */
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* w = glfwCreateWindow(640, 480, "Sokol Triangle GLFW", 0, 0);
    glfwMakeContextCurrent(w);
    glfwSwapInterval(1);

    /* setup sokol_gfx */
    sg_setup(&(sg_desc){
        .logger.func = slog_func,
    });
    simgui_setup(&(simgui_desc_t){ 0 });

    /* a vertex buffer */
    const float vertices[] = {
        // positions            // colors
         0.0f,  0.5f, 0.5f,     1.0f, 0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, 0.5f,     0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f,     0.0f, 0.0f, 1.0f, 1.0f
    };
    sg_buffer vbuf = sg_make_buffer(&(sg_buffer_desc){
        .data = SG_RANGE(vertices)
    });

    /* a shader */
    sg_shader shd = sg_make_shader(&(sg_shader_desc){
        .vs.source =
            "#version 330\n"
            "layout(location=0) in vec4 position;\n"
            "layout(location=1) in vec4 color0;\n"
            "out vec4 color;\n"
            "void main() {\n"
            "  gl_Position = position;\n"
            "  color = color0;\n"
            "}\n",
        .fs.source =
            "#version 330\n"
            "in vec4 color;\n"
            "out vec4 frag_color;\n"
            "void main() {\n"
            "  frag_color = color;\n"
            "}\n"
    });

    /* a pipeline state object (default render states are fine for triangle) */
    sg_pipeline pip = sg_make_pipeline(&(sg_pipeline_desc){
        .shader = shd,
        .layout = {
            .attrs = {
                [0].format=SG_VERTEXFORMAT_FLOAT3,
                [1].format=SG_VERTEXFORMAT_FLOAT4
            }
        }
    });

    /* resource bindings */
    sg_bindings bind = {
        .vertex_buffers[0] = vbuf
    };

    /* default pass action (clear to grey) */
    sg_pass_action pass_action = {0};

    /* draw loop */
    while (!glfwWindowShouldClose(w)) {
        int cur_width, cur_height;
        glfwGetFramebufferSize(w, &cur_width, &cur_height);
        sg_begin_default_pass(&pass_action, cur_width, cur_height);
        sg_apply_pipeline(pip);
        sg_apply_bindings(&bind);
        sg_draw(0, 3, 1);
        simgui_new_frame(&(simgui_frame_desc_t){
            .width = sapp_width(),
            .height = sapp_height(),
            .delta_time = sapp_frame_duration(),
            .dpi_scale = sapp_dpi_scale(),
        });

        /*=== UI CODE STARTS HERE ===*/
        igSetNextWindowPos((ImVec2){10,10}, ImGuiCond_Once, (ImVec2){0,0});
        igSetNextWindowSize((ImVec2){400, 100}, ImGuiCond_Once);
        igBegin("Hello Dear ImGui!", 0, ImGuiWindowFlags_None);
        igColorEdit3("Background", &state.pass_action.colors[0].value.r, ImGuiColorEditFlags_None);
        igEnd();
        /*=== UI CODE ENDS HERE ===*/

        sg_begin_default_pass(&state.pass_action, sapp_width(), sapp_height());
        simgui_render();
        sg_end_pass();
        sg_commit();
        glfwSwapBuffers(w);
        glfwPollEvents();
    }

    /* cleanup */
    simgui_shutdown();
    sg_shutdown();
    glfwTerminate();
    return 0;
}