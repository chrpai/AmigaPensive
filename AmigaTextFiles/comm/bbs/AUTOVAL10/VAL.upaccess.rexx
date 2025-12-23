/* Update access group
 *
 *   Called by Excelsior! Autoval v1.0
 */

options results

getuser 15;access=result
if access~=1 then exit
setaccess 3
