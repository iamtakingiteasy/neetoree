/*
 * Generated by gdbus-codegen 2.46.2. DO NOT EDIT.
 *
 * The license of this code is the same as for the source it was derived from.
 */

#ifndef ___HOME_USER_SOFT_MINE_NEETOREE_DIRECTINT_SRC_GEN_DBUSAPI_H__
#define ___HOME_USER_SOFT_MINE_NEETOREE_DIRECTINT_SRC_GEN_DBUSAPI_H__

#include <gio/gio.h>

G_BEGIN_DECLS


/* ------------------------------------------------------------------------ */
/* Declarations for com.Skype.API */

#define SKYPE_TYPE_API (skype_api_get_type ())
#define SKYPE_API(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), SKYPE_TYPE_API, SkypeAPI))
#define SKYPE_IS_API(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), SKYPE_TYPE_API))
#define SKYPE_API_GET_IFACE(o) (G_TYPE_INSTANCE_GET_INTERFACE ((o), SKYPE_TYPE_API, SkypeAPIIface))

struct _SkypeAPI;
typedef struct _SkypeAPI SkypeAPI;
typedef struct _SkypeAPIIface SkypeAPIIface;

struct _SkypeAPIIface
{
  GTypeInterface parent_iface;

  gboolean (*handle_invoke) (
    SkypeAPI *object,
    GDBusMethodInvocation *invocation,
    const gchar *arg_request);

};

GType skype_api_get_type (void) G_GNUC_CONST;

GDBusInterfaceInfo *skype_api_interface_info (void);
guint skype_api_override_properties (GObjectClass *klass, guint property_id_begin);


/* D-Bus method call completion functions: */
void skype_api_complete_invoke (
    SkypeAPI *object,
    GDBusMethodInvocation *invocation,
    const gchar *result);



/* D-Bus method calls: */
void skype_api_call_invoke (
    SkypeAPI *proxy,
    const gchar *arg_request,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean skype_api_call_invoke_finish (
    SkypeAPI *proxy,
    gchar **out_result,
    GAsyncResult *res,
    GError **error);

gboolean skype_api_call_invoke_sync (
    SkypeAPI *proxy,
    const gchar *arg_request,
    gchar **out_result,
    GCancellable *cancellable,
    GError **error);



/* ---- */

#define SKYPE_TYPE_API_PROXY (skype_api_proxy_get_type ())
#define SKYPE_API_PROXY(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), SKYPE_TYPE_API_PROXY, SkypeAPIProxy))
#define SKYPE_API_PROXY_CLASS(k) (G_TYPE_CHECK_CLASS_CAST ((k), SKYPE_TYPE_API_PROXY, SkypeAPIProxyClass))
#define SKYPE_API_PROXY_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), SKYPE_TYPE_API_PROXY, SkypeAPIProxyClass))
#define SKYPE_IS_API_PROXY(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), SKYPE_TYPE_API_PROXY))
#define SKYPE_IS_API_PROXY_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), SKYPE_TYPE_API_PROXY))

typedef struct _SkypeAPIProxy SkypeAPIProxy;
typedef struct _SkypeAPIProxyClass SkypeAPIProxyClass;
typedef struct _SkypeAPIProxyPrivate SkypeAPIProxyPrivate;

struct _SkypeAPIProxy
{
  /*< private >*/
  GDBusProxy parent_instance;
  SkypeAPIProxyPrivate *priv;
};

struct _SkypeAPIProxyClass
{
  GDBusProxyClass parent_class;
};

GType skype_api_proxy_get_type (void) G_GNUC_CONST;

void skype_api_proxy_new (
    GDBusConnection     *connection,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GAsyncReadyCallback  callback,
    gpointer             user_data);
SkypeAPI *skype_api_proxy_new_finish (
    GAsyncResult        *res,
    GError             **error);
SkypeAPI *skype_api_proxy_new_sync (
    GDBusConnection     *connection,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GError             **error);

void skype_api_proxy_new_for_bus (
    GBusType             bus_type,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GAsyncReadyCallback  callback,
    gpointer             user_data);
SkypeAPI *skype_api_proxy_new_for_bus_finish (
    GAsyncResult        *res,
    GError             **error);
SkypeAPI *skype_api_proxy_new_for_bus_sync (
    GBusType             bus_type,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GError             **error);


/* ---- */

#define SKYPE_TYPE_API_SKELETON (skype_api_skeleton_get_type ())
#define SKYPE_API_SKELETON(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), SKYPE_TYPE_API_SKELETON, SkypeAPISkeleton))
#define SKYPE_API_SKELETON_CLASS(k) (G_TYPE_CHECK_CLASS_CAST ((k), SKYPE_TYPE_API_SKELETON, SkypeAPISkeletonClass))
#define SKYPE_API_SKELETON_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), SKYPE_TYPE_API_SKELETON, SkypeAPISkeletonClass))
#define SKYPE_IS_API_SKELETON(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), SKYPE_TYPE_API_SKELETON))
#define SKYPE_IS_API_SKELETON_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), SKYPE_TYPE_API_SKELETON))

typedef struct _SkypeAPISkeleton SkypeAPISkeleton;
typedef struct _SkypeAPISkeletonClass SkypeAPISkeletonClass;
typedef struct _SkypeAPISkeletonPrivate SkypeAPISkeletonPrivate;

struct _SkypeAPISkeleton
{
  /*< private >*/
  GDBusInterfaceSkeleton parent_instance;
  SkypeAPISkeletonPrivate *priv;
};

struct _SkypeAPISkeletonClass
{
  GDBusInterfaceSkeletonClass parent_class;
};

GType skype_api_skeleton_get_type (void) G_GNUC_CONST;

SkypeAPI *skype_api_skeleton_new (void);


/* ------------------------------------------------------------------------ */
/* Declarations for com.Skype.API.Client */

#define SKYPE_TYPE_APICLIENT (skype_apiclient_get_type ())
#define SKYPE_APICLIENT(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), SKYPE_TYPE_APICLIENT, SkypeAPIClient))
#define SKYPE_IS_APICLIENT(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), SKYPE_TYPE_APICLIENT))
#define SKYPE_APICLIENT_GET_IFACE(o) (G_TYPE_INSTANCE_GET_INTERFACE ((o), SKYPE_TYPE_APICLIENT, SkypeAPIClientIface))

struct _SkypeAPIClient;
typedef struct _SkypeAPIClient SkypeAPIClient;
typedef struct _SkypeAPIClientIface SkypeAPIClientIface;

struct _SkypeAPIClientIface
{
  GTypeInterface parent_iface;

  gboolean (*handle_notify) (
    SkypeAPIClient *object,
    GDBusMethodInvocation *invocation,
    const gchar *arg_message);

};

GType skype_apiclient_get_type (void) G_GNUC_CONST;

GDBusInterfaceInfo *skype_apiclient_interface_info (void);
guint skype_apiclient_override_properties (GObjectClass *klass, guint property_id_begin);


/* D-Bus method call completion functions: */
void skype_apiclient_complete_notify (
    SkypeAPIClient *object,
    GDBusMethodInvocation *invocation);



/* D-Bus method calls: */
void skype_apiclient_call_notify (
    SkypeAPIClient *proxy,
    const gchar *arg_message,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean skype_apiclient_call_notify_finish (
    SkypeAPIClient *proxy,
    GAsyncResult *res,
    GError **error);

gboolean skype_apiclient_call_notify_sync (
    SkypeAPIClient *proxy,
    const gchar *arg_message,
    GCancellable *cancellable,
    GError **error);



/* ---- */

#define SKYPE_TYPE_APICLIENT_PROXY (skype_apiclient_proxy_get_type ())
#define SKYPE_APICLIENT_PROXY(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), SKYPE_TYPE_APICLIENT_PROXY, SkypeAPIClientProxy))
#define SKYPE_APICLIENT_PROXY_CLASS(k) (G_TYPE_CHECK_CLASS_CAST ((k), SKYPE_TYPE_APICLIENT_PROXY, SkypeAPIClientProxyClass))
#define SKYPE_APICLIENT_PROXY_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), SKYPE_TYPE_APICLIENT_PROXY, SkypeAPIClientProxyClass))
#define SKYPE_IS_APICLIENT_PROXY(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), SKYPE_TYPE_APICLIENT_PROXY))
#define SKYPE_IS_APICLIENT_PROXY_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), SKYPE_TYPE_APICLIENT_PROXY))

typedef struct _SkypeAPIClientProxy SkypeAPIClientProxy;
typedef struct _SkypeAPIClientProxyClass SkypeAPIClientProxyClass;
typedef struct _SkypeAPIClientProxyPrivate SkypeAPIClientProxyPrivate;

struct _SkypeAPIClientProxy
{
  /*< private >*/
  GDBusProxy parent_instance;
  SkypeAPIClientProxyPrivate *priv;
};

struct _SkypeAPIClientProxyClass
{
  GDBusProxyClass parent_class;
};

GType skype_apiclient_proxy_get_type (void) G_GNUC_CONST;

void skype_apiclient_proxy_new (
    GDBusConnection     *connection,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GAsyncReadyCallback  callback,
    gpointer             user_data);
SkypeAPIClient *skype_apiclient_proxy_new_finish (
    GAsyncResult        *res,
    GError             **error);
SkypeAPIClient *skype_apiclient_proxy_new_sync (
    GDBusConnection     *connection,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GError             **error);

void skype_apiclient_proxy_new_for_bus (
    GBusType             bus_type,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GAsyncReadyCallback  callback,
    gpointer             user_data);
SkypeAPIClient *skype_apiclient_proxy_new_for_bus_finish (
    GAsyncResult        *res,
    GError             **error);
SkypeAPIClient *skype_apiclient_proxy_new_for_bus_sync (
    GBusType             bus_type,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GError             **error);


/* ---- */

#define SKYPE_TYPE_APICLIENT_SKELETON (skype_apiclient_skeleton_get_type ())
#define SKYPE_APICLIENT_SKELETON(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), SKYPE_TYPE_APICLIENT_SKELETON, SkypeAPIClientSkeleton))
#define SKYPE_APICLIENT_SKELETON_CLASS(k) (G_TYPE_CHECK_CLASS_CAST ((k), SKYPE_TYPE_APICLIENT_SKELETON, SkypeAPIClientSkeletonClass))
#define SKYPE_APICLIENT_SKELETON_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), SKYPE_TYPE_APICLIENT_SKELETON, SkypeAPIClientSkeletonClass))
#define SKYPE_IS_APICLIENT_SKELETON(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), SKYPE_TYPE_APICLIENT_SKELETON))
#define SKYPE_IS_APICLIENT_SKELETON_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), SKYPE_TYPE_APICLIENT_SKELETON))

typedef struct _SkypeAPIClientSkeleton SkypeAPIClientSkeleton;
typedef struct _SkypeAPIClientSkeletonClass SkypeAPIClientSkeletonClass;
typedef struct _SkypeAPIClientSkeletonPrivate SkypeAPIClientSkeletonPrivate;

struct _SkypeAPIClientSkeleton
{
  /*< private >*/
  GDBusInterfaceSkeleton parent_instance;
  SkypeAPIClientSkeletonPrivate *priv;
};

struct _SkypeAPIClientSkeletonClass
{
  GDBusInterfaceSkeletonClass parent_class;
};

GType skype_apiclient_skeleton_get_type (void) G_GNUC_CONST;

SkypeAPIClient *skype_apiclient_skeleton_new (void);


G_END_DECLS

#endif /* ___HOME_USER_SOFT_MINE_NEETOREE_DIRECTINT_SRC_GEN_DBUSAPI_H__ */
