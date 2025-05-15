{ pkgs ? import <nixpkgs> {}}:

pkgs.mkShell {
  buildInputs = with pkgs; [
    pkg-config
    gcc
    gnumake
    gtk3.dev
    glib.dev
  ];
}
