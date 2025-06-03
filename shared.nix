# This file contains code shared by `flake.nix` and `shell.nix`.

{ pkgs }:

{
  nativeBuildInputs = with pkgs; [
    pkg-config
    gcc
    gnumake
  ];

  buildInputs = with pkgs; [
    gtk3.dev
    glib.dev
    vte.dev
    cjson
  ];
}
