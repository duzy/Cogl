/*
 * Cogl
 *
 * An object oriented GL/GLES Abstraction/Utility Layer
 *
 * Copyright (C) 2011 Intel Corporation.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 *
 *
 * Authors:
 *   Neil Roberts <neil@linux.intel.com>
 */

#if !defined(__COGL_H_INSIDE__) && !defined(CLUTTER_COMPILATION)
#error "Only <cogl/cogl.h> can be included directly."
#endif

#ifndef __COGL_SNIPPET_H__
#define __COGL_SNIPPET_H__

#include <glib.h>

G_BEGIN_DECLS

/**
 * SECTION:cogl-snippet
 * @short_description: Functions for creating and manipulating shader snippets
 *
 * ...
 */
typedef struct _CoglSnippet CoglSnippet;

#define COGL_SNIPPET(OBJECT) ((CoglSnippet *)OBJECT)

/* Enumeration of all the hook points that a snippet can be attached
   to within a pipeline. */
/**
 * CoglSnippetHook:
 * @COGL_SNIPPET_HOOK_VERTEX: A hook for the entire vertex processing
 *   stage of the pipeline.
 * @COGL_SNIPPET_HOOK_FRAGMENT: A hook for the entire fragment
 *   processing stage of the pipeline.
 * @COGL_SNIPPET_HOOK_TEXTURE_COORD_TRANSFORM: A hook for applying the
 *   layer matrix to a texture coordinate for a layer.
 * @COGL_SNIPPET_HOOK_LAYER_FRAGMENT: A hook for the fragment
 *   processing of a particular layer.
 * @COGL_SNIPPET_HOOK_TEXTURE_LOOKUP: A hook for the texture lookup
 *   stage of a given layer in a pipeline.
 *
 * #CoglSnippetHook is used to specify a location within a
 * #CoglPipeline where the code of the snippet should be used when it
 * is attached to a pipeline.
 *
 * <glosslist>
 *  <glossentry>
 *   <glossterm>%COGL_SNIPPET_HOOK_VERTEX</glossterm>
 *   <glossdef>
 * <para>
 * Adds a shader snippet that will hook on to the vertex processing
 * stage of the pipeline. This gives a chance for the application to
 * modify the vertex attributes generated by the shader. Typically the
 * snippet will modify cogl_color_out or cogl_position_out builtins.
 * </para>
 * <para>
 * The ‘declarations’ string in @snippet will be inserted in the
 * global scope of the shader. Use this to declare any uniforms,
 * attributes or functions that the snippet requires.
 * </para>
 * <para>
 * The ‘pre’ string in @snippet will be inserted at the top of the
 * main() function before any vertex processing is done.
 * </para>
 * <para>
 * The ‘replace’ string in @snippet will be used instead of the
 * generated vertex processing if it is present. This can be used if
 * the application wants to provide a complete vertex shader and
 * doesn't need the generated output from Cogl.
 * </para>
 * <para>
 * The ‘post’ string in @snippet will be inserted after all of the
 * standard vertex processing is done. This can be used to modify the
 * outputs.
 * </para>
 *   </glossdef>
 *  </glossentry>
 *  <glossentry>
 *   <glossterm>%COGL_SNIPPET_HOOK_FRAGMENT</glossterm>
 *   <glossdef>
 * <para>
 * Adds a shader snippet that will hook on to the fragment processing
 * stage of the pipeline. This gives a chance for the application to
 * modify the fragment color generated by the shader. Typically the
 * snippet will modify cogl_color_out.
 * </para>
 * <para>
 * The ‘declarations’ string in @snippet will be inserted in the
 * global scope of the shader. Use this to declare any uniforms,
 * attributes or functions that the snippet requires.
 * </para>
 * <para>
 * The ‘pre’ string in @snippet will be inserted at the top of the
 * main() function before any fragment processing is done.
 * </para>
 * <para>
 * The ‘replace’ string in @snippet will be used instead of the
 * generated fragment processing if it is present. This can be used if
 * the application wants to provide a complete fragment shader and
 * doesn't need the generated output from Cogl.
 * </para>
 * <para>
 * The ‘post’ string in @snippet will be inserted after all of the
 * standard fragment processing is done. At this point the generated
 * value for the rest of the pipeline state will already be in
 * cogl_color_out so the application can modify the result by altering
 * this variable.
 * </para>
 *   </glossdef>
 *  </glossentry>
 *  <glossentry>
 *   <glossterm>%COGL_SNIPPET_HOOK_TEXTURE_COORD_TRANSFORM</glossterm>
 * Adds a shader snippet that will hook on to the texture coordinate
 * transformation of a particular layer. This can be used to replace
 * the processing for a layer or to modify the results.
 * </para>
 * <para>
 * Within the snippet code for this hook there are two extra
 * variables. The first is a mat4 called cogl_matrix which represents
 * the user matrix for this layer. The second is called cogl_tex_coord
 * and represents the incoming and outgoing texture coordinate. On
 * entry to the hook, cogl_tex_coord contains the value of the
 * corresponding texture coordinate attribute for this layer. The hook
 * is expected to modify this variable. The output will be passed as a
 * varying to the fragment processing stage. The default code will
 * just multiply cogl_matrix by cogl_tex_coord and store the result in
 * cogl_tex_coord.
 * </para>
 * <para>
 * The ‘declarations’ string in @snippet will be inserted in the
 * global scope of the shader. Use this to declare any uniforms,
 * attributes or functions that the snippet requires.
 * </para>
 * <para>
 * The ‘pre’ string in @snippet will be inserted just before the
 * fragment processing for this layer. At this point cogl_tex_coord
 * still contains the value of the texture coordinate attribute.
 * </para>
 * <para>
 * If a ‘replace’ string is given then this will be used instead of
 * the default fragment processing for this layer. The snippet can
 * modify cogl_tex_coord or leave it as is to apply no transformation.
 * </para>
 * <para>
 * The ‘post’ string in @snippet will be inserted just after the
 * transformation. At this point cogl_tex_coord will contain the
 * results of the transformation but it can be further modified by the
 * snippet.
 * </para>
 *  </glossentry>
 *  <glossentry>
 *   <glossterm>%COGL_SNIPPET_HOOK_LAYER_FRAGMENT</glossterm>
 * Adds a shader snippet that will hook on to the fragment processing
 * of a particular layer. This can be used to replace the processing
 * for a layer or to modify the results.
 * </para>
 * <para>
 * Within the snippet code for this hook there is an extra vec4
 * variable called ‘cogl_layer’. This contains the resulting color
 * that will be used for the layer. This can be modified in the ‘post’
 * section or it the default processing can be replaced entirely using
 * the ‘replace’ section.
 * </para>
 * <para>
 * The ‘declarations’ string in @snippet will be inserted in the
 * global scope of the shader. Use this to declare any uniforms,
 * attributes or functions that the snippet requires.
 * </para>
 * <para>
 * The ‘pre’ string in @snippet will be inserted just before the
 * fragment processing for this layer.
 * </para>
 * <para>
 * If a ‘replace’ string is given then this will be used instead of
 * the default fragment processing for this layer. The snippet must write to
 * the ‘cogl_layer’ variable in that case.
 * </para>
 * <para>
 * The ‘post’ string in @snippet will be inserted just after the
 * fragment processing for the layer. The results can be modified by changing
 * the value of the ‘cogl_layer’ variable.
 * </para>
 *  </glossentry>
 *  <glossentry>
 *   <glossterm>%COGL_SNIPPET_HOOK_TEXTURE_LOOKUP</glossterm>
 * Adds a shader snippet that will hook on to the texture lookup part
 * of a given layer. This gives a chance for the application to modify
 * the coordinates that will be used for the texture lookup or to
 * alter the returned texel.
 * </para>
 * <para>
 * Within the snippet code for this hook there are two extra variables
 * available. ‘cogl_tex_coord’ is a vec4 which contains the texture
 * coordinates that will be used for the texture lookup this can be
 * modified. ‘cogl_texel’ will contain the result of the texture
 * lookup. This can be modified.
 * </para>
 * <para>
 * The ‘declarations’ string in @snippet will be inserted in the
 * global scope of the shader. Use this to declare any uniforms,
 * attributes or functions that the snippet requires.
 * </para>
 * <para>
 * The ‘pre’ string in @snippet will be inserted at the top of the
 * main() function before any fragment processing is done. This is a
 * good place to modify the cogl_tex_coord variable.
 * </para>
 * <para>
 * If a ‘replace’ string is given then this will be used instead of a
 * the default texture lookup. The snippet would typically use its own
 * sampler in this case.
 * </para>
 * <para>
 * The ‘post’ string in @snippet will be inserted after texture lookup
 * has been preformed. Here the snippet can modify the cogl_texel
 * variable to alter the returned texel.
 * </para>
 *  </glossentry>
 * </glosslist>
 *
 * Since: 1.10
 * Stability: Unstable
 */
typedef enum {
  /* Per pipeline vertex hooks */
  COGL_SNIPPET_HOOK_VERTEX = 0,

  /* Per pipeline fragment hooks */
  COGL_SNIPPET_HOOK_FRAGMENT = 2048,

  /* Per layer vertex hooks */
  COGL_SNIPPET_HOOK_TEXTURE_COORD_TRANSFORM = 4096,

  /* Per layer fragment hooks */
  COGL_SNIPPET_HOOK_LAYER_FRAGMENT = 6144,
  COGL_SNIPPET_HOOK_TEXTURE_LOOKUP
} CoglSnippetHook;

#define cogl_snippet_new cogl_snippet_new_EXP
/**
 * cogl_snippet_new:
 * @hook: The point in the pipeline that this snippet will wrap around
 *   or replace.
 * @declarations: The source code for the declarations for this
 *   snippet or %NULL. See cogl_snippet_set_declarations().
 * @post: The source code to run after the hook point where this
 *   shader snippet is attached or %NULL. See cogl_snippet_set_post().
 *
 * Allocates and initializes a new snippet with the given source strings.
 *
 * Return value: a pointer to a new #CoglSnippet
 *
 * Since: 1.10
 * Stability: Unstable
 */
CoglSnippet *
cogl_snippet_new (CoglSnippetHook hook,
                  const char *declarations,
                  const char *post);

#define cogl_snippet_get_hook cogl_snipet_get_hook_EXP
/**
 * cogl_snippet_get_hook:
 * @snippet: A #CoglSnippet
 *
 * Return value: the hook that was set when cogl_snippet_new() was
 * called.
 */
CoglSnippetHook
cogl_snippet_get_hook (CoglSnippet *snippet);

#define cogl_is_snippet cogl_is_snippet_EXP
/**
 * cogl_is_snippet:
 * @handle: A CoglHandle
 *
 * Gets whether the given handle references an existing snippet object.
 *
 * Return value: %TRUE if the handle references a #CoglSnippet,
 *   %FALSE otherwise
 *
 * Since: 1.10
 * Stability: Unstable
 */
gboolean
cogl_is_snippet (void *object);

#define cogl_snippet_set_declarations cogl_snippet_set_declarations_EXP
/**
 * cogl_snippet_set_declarations:
 * @snippet: A #CoglSnippet
 * @declarations: The new source string for the declarations section
 *   of this snippet.
 *
 * Sets a source string that will be inserted in the global scope of
 * the generated shader when this snippet is used on a pipeline. This
 * string is typically used to declare uniforms, attributes or
 * functions that will be used by the other parts of the snippets.
 *
 * This function should only be called before the snippet is attached
 * to its first pipeline. After that the snippet should be considered
 * immutable.
 *
 * Since: 1.10
 * Stability: Unstable
 */
void
cogl_snippet_set_declarations (CoglSnippet *snippet,
                               const char *declarations);

#define cogl_snippet_get_declarations cogl_snippet_get_declarations_EXP
/**
 * cogl_snippet_get_declarations:
 * @snippet: A #CoglSnippet
 *
 * Return value: the source string that was set with
 *   cogl_snippet_set_declarations() or %NULL if none was set.
 *
 * Since: 1.10
 * Stability: Unstable
 */
const char *
cogl_snippet_get_declarations (CoglSnippet *snippet);

#define cogl_snippet_set_pre cogl_snippet_set_pre_EXP
/**
 * cogl_snippet_set_pre:
 * @snippet: A #CoglSnippet
 * @pre: The new source string for the pre section of this snippet.
 *
 * Sets a source string that will be inserted before the hook point in
 * the generated shader for the pipeline that this snippet is attached
 * to. Please see the documentation of each hook point in
 * #CoglPipeline for a description of how this string should be used.
 *
 * This function should only be called before the snippet is attached
 * to its first pipeline. After that the snippet should be considered
 * immutable.
 *
 * Since: 1.10
 * Stability: Unstable
 */
void
cogl_snippet_set_pre (CoglSnippet *snippet,
                      const char *pre);

#define cogl_snippet_get_pre cogl_snippet_get_pre_EXP
/**
 * cogl_snippet_get_pre:
 * @snippet: A #CoglSnippet
 *
 * Return value: the source string that was set with
 *   cogl_snippet_set_pre() or %NULL if none was set.
 *
 * Since: 1.10
 * Stability: Unstable
 */
const char *
cogl_snippet_get_pre (CoglSnippet *snippet);

#define cogl_snippet_set_replace cogl_snippet_set_replace_EXP
/**
 * cogl_snippet_set_replace:
 * @snippet: A #CoglSnippet
 * @replace: The new source string for the replace section of this snippet.
 *
 * Sets a source string that will be used instead of any generated
 * source code or any previous snippets for this hook point. Please
 * see the documentation of each hook point in #CoglPipeline for a
 * description of how this string should be used.
 *
 * This function should only be called before the snippet is attached
 * to its first pipeline. After that the snippet should be considered
 * immutable.
 *
 * Since: 1.10
 * Stability: Unstable
 */
void
cogl_snippet_set_replace (CoglSnippet *snippet,
                          const char *replace);

#define cogl_snippet_get_replace cogl_snippet_get_replace_EXP
/**
 * cogl_snippet_get_replace:
 * @snippet: A #CoglSnippet
 *
 * Return value: the source string that was set with
 *   cogl_snippet_set_replace() or %NULL if none was set.
 *
 * Since: 1.10
 * Stability: Unstable
 */
const char *
cogl_snippet_get_replace (CoglSnippet *snippet);

#define cogl_snippet_set_post cogl_snippet_set_post_EXP
/**
 * cogl_snippet_set_post:
 * @snippet: A #CoglSnippet
 * @post: The new source string for the post section of this snippet.
 *
 * Sets a source string that will be inserted after the hook point in
 * the generated shader for the pipeline that this snippet is attached
 * to. Please see the documentation of each hook point in
 * #CoglPipeline for a description of how this string should be used.
 *
 * This function should only be called before the snippet is attached
 * to its first pipeline. After that the snippet should be considered
 * immutable.
 *
 * Since: 1.10
 * Stability: Unstable
 */
void
cogl_snippet_set_post (CoglSnippet *snippet,
                       const char *post);

#define cogl_snippet_get_post cogl_snippet_get_post_EXP
/**
 * cogl_snippet_get_post:
 * @snippet: A #CoglSnippet
 *
 * Return value: the source string that was set with
 *   cogl_snippet_set_post() or %NULL if none was set.
 *
 * Since: 1.10
 * Stability: Unstable
 */
const char *
cogl_snippet_get_post (CoglSnippet *snippet);

G_END_DECLS

#endif /* __COGL_SNIPPET_H__ */
