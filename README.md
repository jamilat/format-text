# format_text
<H1>About</H1>
  <body>
  A program created in my Software Development Methods class.<br><br> This is a C	program	that	inputs	lines	from stdin,	reads formatting	options	
contained	within	that	stream	of	characters,	and	then	outputs	to	stdout the text	
where	each	line	output	has	the	appropriate	width,	indenting,	etc.	as	indicated by	the	
formatting	commands.
    </body>
  <H2>Compilation</H2>
    <body>gcc -Wall senjify1.c -o senjify1</body>
  <H2>Execution</H2>
    <body> Assuming that your input file is in the same directory as your senjify1.c file: <br> cat input.txt | ./senjify1 > output.txt </body>
  <H2>Expected Output testing</H2>
    <body> Assuming that your input file is in the same directory as your output file and senjify1.c file: <br> cat input.txt | ./senjify1 | diff – tests/output.txt </body>
  <H2>Examples</H2>
    <body>Input files within this repository:<br>
      in08.txt in09.txt in10.txt<br><br>
      Output files within this repository:<br>
      out08.txt out09.txt out10.txt
      </body>
