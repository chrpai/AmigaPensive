void FileDate(struct DateStamp *dstamp, struct IsDate *CNDate)
{
   LONG Date   [ 3 ];
   char UPDate [ 6 ];

   Date[0] = dstamp->ds_Days * 24 * 60;
   Date[1] = (dstamp->ds_Minute + Date [ 0 ]) * 60;
   Date[2] = ((dstamp->ds_Tick/50) + Date [ 1 ]);

   utunpk(Date[2], UPDate);

   UPDate [ 0 ] += 78;

   CNDate->Year   = UPDate[ 0 ];
   CNDate->Month  = UPDate[ 1 ];
   CNDate->Date   = UPDate[ 2 ];
   CNDate->Hour   = UPDate[ 3 ];
   CNDate->Minute = UPDate[ 4 ];
   CNDate->Second = UPDate[ 5 ];
}
