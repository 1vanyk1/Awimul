#ifndef _GLX_dri_common_h
#define _GLX_dri_common_h

typedef struct __GLXDRIconfig __GLXDRIconfig;
struct __GLXDRIconfig {
    __GLXconfig config;
    const __DRIconfig *driConfig;
};

__GLXconfig *glxConvertConfigs(const __DRIcoreExtension * core,
                               const __DRIconfig ** configs);

void *glxProbeDriver(const char *name,
                     void **coreExt, const char *coreName, int coreVersion,
                     void **renderExt, const char *renderName,
                     int renderVersion);

#endif
