#!/bin/bash

echo "──────────────────────────────────────────────"
echo "        Installing ShrijiLang v1.0"
echo "──────────────────────────────────────────────"

INSTALL_DIR="$HOME/shrijilang"

echo "Creating directory at $INSTALL_DIR..."
mkdir -p "$INSTALL_DIR/bin"

echo "Copying binary..."
cp linux/bin/shrijilang "$INSTALL_DIR/bin/"

chmod +x "$INSTALL_DIR/bin/shrijilang"

echo ""
echo "Installation complete!"
echo "Run ShrijiLang using:"
echo ""
echo "    $INSTALL_DIR/bin/shrijilang"
echo ""
echo "──────────────────────────────────────────────"
