# html2latex

<details open="open">
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
  </ol>
</details>

## About the Project
html2latex can translate any html document into a LaTeX file. It extracts the 
textual content from any html document and uses this to create a .tex file 
that can be easily compiled using any LaTeX compiler.

### Built with
* C++
* cURL

## Getting started
Setting up the project is pretty simple. You'll need to have a working C++ compiler
and a LaTeX compiler to translate the .tex file into a PDF.

### Prerequisites

html2latex uses the C++ cURL library for fetching the HTML at any specified URL.
This uses the OpenSSL libraries that need to be installed on your system.
If this isn't installed use any of the the following commands depending on the
OS you're using:

#### Ubuntu
```
sudo aptitude install libcurl4-openssl-dev
```
#### Arch
```
yay -Y libcurl-openssl-1.0
```
<br />
There is some problem with the GPG key and it has to imported manually from https://daniel.haxx.se/mykey.asc as
specified <a href="https://aur.archlinux.org/packages/libcurl-openssl-1.0/">here</a>

### Installation
#### Clone the repo
```
git clone https://github.com/fizlip/html2latex.git
```

## Usage
### Compile and run the code 

```
g++ main.cpp -o html2latex_example-lcurl
./html2latex_example
```

The program will prompt you for a filename to store the text data in.
This should be a .tex file if you want to compile it with a LaTeX compiler.
After this the program will ask for a URL that contains the HTML you want to
convert. Once this information is entered html2latex will do its magic and 
pop out a .tex file that can be compiled into an easy to read PDF.

#### Example
```
Enter a filename: test.tex
Enter a URL: https://example.com/cool_article.html
```

## Contributing

Contributions are what make the open source community such an amazing place to be learn, inspire, and create. Any contributions you make are **greatly appreciated**.

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## License

Distributed under the MIT License. See `LICENSE` for more information.

## Contact

Filip Zlatoidsky - filip@zlato.xyz

Project Link: https://github.com/fizlip/html2latex.git
