<h1 align="center">
  Zoe
</h1>

<p align="center">
  Zoe is a video game engine that I am working on in my free time. It probably won't surpass <i>Unity3D</i> or <i>Unreal Engine</i> but where is the fun in using any of these if you can make your own! <br>I want it to be as optimized as possible but am limited to my free time.
</p>

<h3 align="center">
  <img src="https://github.com/NudelErde/zoe/workflows/Build%20(with%20CMake)/badge.svg">
  <a href="https://app.fossa.com/projects/git%2Bgithub.com%2FNudelErde%2Fzoe?ref=badge_shield"><img src="https://app.fossa.com/api/projects/git%2Bgithub.com%2FNudelErde%2Fzoe.svg?type=shield"></a>
</h3>


# Compiling
I don't know why you would like to compile zoe at its current state, but...

You can compile zoe [as we tell GitHub to](./.github/workflows/cmake.yml).
* Make sure you are using a Linux or Windows machine to compile and run zoe
* Install all dependencies `apt install build-essential cmake libglew-dev libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libgl-dev`

1. Clone the repository with `git clone --recurse-submodules https://github.com/NudelErde/zoe.git` (Or use `git submodule update --init --recursive` when you already cloned)
2. Run `cmake .` and let *cmake* prepare everything
3. Run `cmake --build .` to build zoe

Your artifacts are located in `./sandbox/sandbox` and `./zoe/libzoe.so`

## Running
Zoe is still work in progress. You can try running [the sandbox](https://github.com/NudelErde/zoe/releases) but please don't expect anything other than a simple demo.

## Contributors ✨
<table>
  <tr>
    <td align="center"><a href="https://github.com/Sprax2013"><img src="https://avatars1.githubusercontent.com/u/7331598?s=460&v=4" width="100px" alt=""><br><sub><b>Sprax</b></sub></a><br>📝 Helps with non-code Stuff</td>
  </tr>
</table>

## Licenses
zoe is licensed under the [MIT License](https://github.com/NudelErde/zoe/blob/master/LICENSE)

**Please take a look at the licenses of [our dependencies](https://github.com/NudelErde/zoe/tree/master/dependencies)** in particular [ChaiScript](https://github.com/ChaiScript/ChaiScript/blob/3af55d60f23c6a3ef4e3becc16ade6364ca15f7d/LICENSE)
