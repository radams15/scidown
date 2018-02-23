Scidown - AST Generator
=======================

Simple example code:

```markdown
# TITLE
Lorem ipsum *dolor* sit amet, consectetur adipiscing elit.
Curabitur eleifend dui eu tellus elementum auctor.

## SECTION I
Donec egestas vitae **neque** ut molestie.
Donec fermentum placerat lectus.

## SECTION II
Mavens ut dui nisi. Nullam tincidunt aliquet augue, a consectetur ***justo*** varius ut.

```

`AST` generated

```
[Root]  {"# TITLE/Lorem...", 311} (0 - 310)
	 [Section]  {"# TITLE/Lorem...", 311} (0 - 310)
		 [H-Title]  {"TITLE", 5} (2 - 6)
		 [Text]  {"Lorem ipsum ", 12} (8 - 19)
		 [Italic]  {"*dolor*", 7} (20 - 26)
			 [Text]  {"dolor", 5} (21 - 25)
		 [Text]  {" sit amet, co...", 91} (27 - 117)
		 [Sub-section]  {"## SECTION I/...", 89} (118 - 206)
			 [H-Title]  {"SECTION I", 9} (121 - 129)
			 [Text]  {"Donec egestas...", 20} (131 - 150)
			 [Bold]  {"**neque**", 9} (151 - 159)
				 [Text]  {"neque", 5} (153 - 157)
			 [Text]  {" ut molestie....", 47} (160 - 206)
		 [Sub-section]  {"## SECTION II...", 104} (207 - 310)
			 [H-Title]  {"SECTION II", 10} (210 - 219)
			 [Text]  {"Maecenas ut d...", 67} (221 - 287)
			 [Bold]  {"***justo***", 11} (288 - 298)
				 [Italic]  {"*justo*", 7} (290 - 296)
					 [Text]  {"justo", 5} (291 - 295)
			 [Text]  {" varius ut./", 12} (299 - 310)
```
