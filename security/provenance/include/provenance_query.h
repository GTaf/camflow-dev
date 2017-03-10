/*
 *
 * Author: Thomas Pasquier <tfjmp@g.harvard.edu>
 *
 * Copyright (C) 2017 Harvard University
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2, as
 * published by the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 */
 #ifndef CONFIG_SECURITY_PROVENANCE_QUERY_H
 #define CONFIG_SECURITY_PROVENANCE_QUERY_H
 #include <linux/camflow_query.h>

 static inline int call_camflow_out_edge(union prov_msg* node,
 					union prov_msg* edge)
 {
 	int rc = 0;
 	struct list_head *listentry, *listtmp;
 	struct policy_hook *fcn;

 	list_for_each_safe(listentry, listtmp, &policy_hooks) {
 		fcn = list_entry(listentry, struct policy_hook, list);
 		if (fcn->out_edge)
 			rc |= fcn->out_edge(node, edge);
 	}
 	return rc;
 }

 static inline int call_camflow_in_edge(union prov_msg* edge,
 				       union prov_msg* node)
 {
 	int rc = 0;
 	struct list_head *listentry, *listtmp;
 	struct policy_hook *fcn;

 	list_for_each_safe(listentry, listtmp, &policy_hooks) {
 		fcn = list_entry(listentry, struct policy_hook, list);
 		if (fcn->in_edge)
 			rc |= fcn->in_edge(edge, node);
 	}
 	return rc;
 }

 static inline int call_query_hooks(union prov_msg *from,
 				union prov_msg *to,
 				union prov_msg *edge)
 {
 	int rc = 0;

 	rc = call_camflow_out_edge(from, edge);
 	rc |= call_camflow_in_edge(edge, to);
 	if ( (rc & CAMFLOW_RAISE_WARNING) == CAMFLOW_RAISE_WARNING) {
 		printk(KERN_WARNING "Provenance warning raised.\n");
 	}
 	if ( (rc & CAMFLOW_PREVENT_FLOW) == CAMFLOW_PREVENT_FLOW) {
 		edge->relation_info.allowed = FLOW_DISALLOWED;
 		return -EPERM;
 	}
 	return 0;
 }

 #endif
