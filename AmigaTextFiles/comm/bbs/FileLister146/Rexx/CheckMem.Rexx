/* Testet auf groessten Speicherblock */

minblock=100000 /* minimale Groesse des groessten zusammenhaengender
                   Speicherblocks */
minmem=1000000  /* minimal freier Speicher */

memfile='t:___checkmem'

address command 'avail >'||memfile

if open(checkmem,memfile,'R') then do
  zeile=readln(checkmem)
  zeile=readln(checkmem)
  zeile=readln(checkmem)
  zeile=readln(checkmem)
  maxmem=strip(word(zeile,2))
  maxblock=strip(word(zeile,5))

  if maxmem<minmem then do
    say '!!! ACHTUNG !!! Speicherplatzmangel ! Nur noch max. '||maxmem||' Bytes frei !'
  end
  else do
    say 'Es sind noch max. '||maxmem||' Bytes frei !'
  end

  if maxblock<minblock then do
    say '!!! ACHTUNG !!! Der groesste zusammenhaenger Speicherblock ist nur noch '||maxblock||' Bytes gross !'
  end
  else do
    say 'Der groesste zusammenhaenger Speicherblock ist noch '||maxblock||' Bytes gross !'
  end

  close(checkmem)
end
else
  say '!! FEHLER !! Konnte Memfile nicht oeffnen !!'
