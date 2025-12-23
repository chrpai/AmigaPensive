/*
** DumpTemplates.rexx
*/

Options Results

Address 'ZEUS'

i   = 0
fin = 0

do until fin = 1
	GetTemplate 'Ordinal' i 'AccessLvl'
	if RC = 0 then do
		Access = RESULT
		Access = Right( Access, 3 )

		GetTemplate 'Ordinal' i 'Description'
		Description = RESULT

		say 'Template 'i': 'Access' 'Description

		ranges = 0
		ord    = 0

		say 'File Ranges:'

		do until ranges = 1
			GetTemplate 'Ordinal' i 'FileRange' ord 'Low'
			if RC = 0 then do
				Low = RESULT
				Low = Right( Low, 7 ) 

				GetTemplate 'Ordinal' i 'FileRange' ord 'High'
				High = RESULT
				High = Left( High, 7 ) 

				GetTemplate 'Ordinal' i 'FileRange' ord 'Flags'
				Flags = RESULT

				say Low'-'High' 'Flags

				end
			else
				ranges = 1

			ord = ord + 1
			end

		end
	else
		fin = 1
	i = i + 1
	end

