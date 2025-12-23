/** setup.rexx
*
*   This adds various libraries to ARexx's list, including
*   QuickSort. Run this in your startup sequence or use it
*   as an example.
*
**/
check = addlib('rexxsupport.library',0,-30,0) 
check = addlib('rexxmathlib.library',0,-30,0)
check = addlib('rexxarplib.library',0,-30,0)

/*
*   Add the QuickSort port, so we can use QSORT.
*   First check if the port is already up. If not, run QuickSort
*   and wait for the port to be there.
*/
if ~showlist('p','QuickSortPort') then
do
   ADDRESS COMMAND "run quicksort >NIL:"
   do i = 1 to 10
      if ~showlist('p','QuickSortPort') then call delay 20
      else leave i
   end
   if showlist('p','QuickSortPort') then call addlib('QuickSortPort',-30)   
end
exit 0
