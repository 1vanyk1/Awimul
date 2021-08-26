#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "libxfontint.h"
#include "replace.h"
#include "../headers/fonts/fntfilst.h"

static FontRenderersRec	renderers;

/*
 * XXX Maybe should allow unregistering renders. For now, just clear the
 * list at each new generation.
 */
static unsigned long rendererGeneration = 0;

Bool
FontFileRegisterRenderer (FontRendererPtr renderer)
{
    return FontFilePriorityRegisterRenderer(renderer, 0);
}

Bool
FontFilePriorityRegisterRenderer (FontRendererPtr renderer, int priority)
{
    int		    i;
    struct _FontRenderersElement *_new;

    if (rendererGeneration != __GetServerGeneration()) {
        rendererGeneration = __GetServerGeneration();
        renderers.number = 0;
        if (renderers.renderers)
            free(renderers.renderers);
        renderers.renderers = NULL;
    }

    for (i = 0; i < renderers.number; i++) {
        if (!strcasecmp (renderers.renderers[i].renderer->fileSuffix,
                         renderer->fileSuffix)) {
            if(renderers.renderers[i].priority >= priority) {
                if(renderers.renderers[i].priority == priority) {
                    if (rendererGeneration == 1)
                        ErrorF("Warning: font renderer for \"%s\" "
                               "already registered at priority %d\n",
                               renderer->fileSuffix, priority);
                }
                return TRUE;
            } else {
                break;
            }
        }
    }

    if(i >= renderers.number) {
        _new = reallocarray (renderers.renderers, i + 1, sizeof(*_new));
        if (!_new)
            return FALSE;
        renderers.renderers = _new;
        renderers.number = i + 1;
    }
    renderer->number = i;
    renderers.renderers[i].renderer = renderer;
    renderers.renderers[i].priority = priority;
    return TRUE;
}

FontRendererPtr
FontFileMatchRenderer (char *fileName)
{
    int			i;
    int			fileLen;
    FontRendererPtr	r;

    fileLen = strlen (fileName);
    for (i = 0; i < renderers.number; i++)
    {
        r = renderers.renderers[i].renderer;
        if (fileLen >= r->fileSuffixLen &&
            !strcasecmp (fileName + fileLen - r->fileSuffixLen, r->fileSuffix))
        {
            return r;
        }
    }
    return 0;
}
