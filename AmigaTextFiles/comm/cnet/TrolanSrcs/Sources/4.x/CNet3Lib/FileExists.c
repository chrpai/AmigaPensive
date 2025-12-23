BOOL FileExists(char *filename)
{
   BPTR lock;

   if(lock = Lock(filename, ACCESS_READ))
   {
      UnLock(lock);
      return TRUE;
   }
   else
   {
      return FALSE;
   }
}
