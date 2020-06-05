defmodule Bzip2.MixProject do
  use Mix.Project

  def project do
    [
      app: :bzip2,
      version: "0.0.1",
      elixir: "~> 1.9",
      start_permanent: Mix.env() == :prod,
      deps: deps()
    ]
  end

  # Run "mix help compile.app" to learn about applications.
  def application do
    [
      extra_applications: [:logger]
    ]
  end

  # Run "mix help deps" to learn about dependencies.
  defp deps do
    [
      {:libbzip2, git: "https://sourceware.org/git/bzip2.git", tag: "bzip2-1.0.8", app: false}
    ]
  end
end
