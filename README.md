# 🌸 ShrijiLang

ShrijiLang ek independent scripting language hai jo poori tarah zero se design aur implement ki ja rahi hai.  
Iska goal kisi existing language ko replace karna nahi, balki ek samajhne layak, predictable aur future-ready foundation banana hai.

ShrijiLang C language mein likhi gayi hai aur iska apna lexer, parser, AST (Abstract Syntax Tree), aur interpreter hai — bina kisi external runtime, VM, ya hidden engine ke.

---

## 🎯 Design Intention

ShrijiLang ka core principle:

Language pehle sahi ho, phir badi ho.

Iska matlab:

- unclear behavior allow nahi kiya jaata  
- silently fail karna mana hai  
- jo feature exist karta hai, woh predictable hota hai  

Is language mein “magic behavior” avoid kiya gaya hai.  
Agar koi cheez supported nahi hai, to woh clearly defined hoti hai.

---

## ⚙️ Current State

ShrijiLang abhi ek small but controlled execution language hai.

Supported features:

- Variables (explicit declaration ke saath)  
- Arithmetic expressions  
- Comparison expressions  
- Conditional execution (agar)  
- Looping (jabtak)  
- Block-based execution  
- .sri script files  
- Test-driven runtime verification  

Sab kuch direct C implementation hai —  
na bytecode, na VM, na external dependency.

---

## 🧠 Execution Model

ShrijiLang ka interpreter:

- galat input ko ignore nahi karta  
- unclear syntax ko future feature maan kar skip nahi karta  
- execution ke dauraan hidden behavior change nahi karta  

Har statement ka ek clear outcome hota hai:

- ya to execute hota hai  
- ya explicit error deta hai  

Goal: dheere grow karna, par kabhi tootna nahi.

---

## 🧪 Example

File: hello.sri

mavi x = 10
mavi y = 5
bolo x + y

Run:

./shrijilang
import "hello.sri"

Output:

15

---

## 📂 Project Structure

ShrijiLang/
├── src/        core language (lexer, parser, interpreter)
├── include/    headers
├── env/        environment system
├── tests/      runtime verification
├── pragya/     AI modules
├── krst/       intelligence layer
├── Makefile
└── shrijilang

AI-related folders execution engine ka hissa nahi hain.  
Ye sirf upper-layer systems hain.

---

## 🚀 Release Roadmap

Pre-Alpha (Current)
- core syntax defined
- interpreter stability
- internal testing
- no guarantees

Alpha
- type clarity
- functions support
- better error reporting

Beta
- runtime freeze
- feature stability
- real-world script support

Stable (v1.0)
- language rules freeze
- documentation complete
- long-term support

---

## 🧑‍💻 Development Reality

ShrijiLang ek single independent developer ke dwara build ho rahi hai:

- bina funding
- bina shortcuts
- bina hype

Progress slow ho sakti hai — par strong hogi.

---

## 📜 License

ShrijiLang GNU AGPLv3 license ke under released hai.

Iska matlab:
- source code open rahega
- modifications share karni hongi
- core kabhi closed nahi hoga

Goal: language community ke liye rahe.
