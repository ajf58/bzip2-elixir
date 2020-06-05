defmodule Bzip2 do
  @moduledoc """
  Documentation for Bzip2.
  """

  @on_load {:init, 0}

  app = Mix.Project.config()[:app]

  def init do
    path = :filename.join(:code.priv_dir(unquote(app)), 'bzip2_nif')
    :ok = :erlang.load_nif(path, 0)
  end

  @doc ~S"""
  Get the version of the underlying libbzip2 C library:

      iex> Bzip2.libVersion
      '1.0.8, 13-Jul-2019'
  """
  @spec libVersion() :: String.t()
  def libVersion() do
    raise "NIF libVersion/0 not implemented"
  end
end
