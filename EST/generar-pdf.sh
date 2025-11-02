#!/bin/bash
# Genera el archivo pdf a partir de la fuente en markdown
pandoc PECEST2425.md --pdf-engine=xelatex -s -f gfm -t pdf --highlight-style=tango -o PEC-EST-24-25-Alejandro\ Muñoz\ Fernández.pdf
