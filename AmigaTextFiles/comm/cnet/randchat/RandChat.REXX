/* RandChat by Wolverine / DMI */

options results

parse arg chat

sf = 'sendfile'
MaxFiles=4
path = 'pfiles:ChatText/'

main:

num = random(1,MaxFiles,time(s))

if chat = 'chat' then do

sf path||'chat.'num
end

else do
sf path||'end.'num
end

exit
