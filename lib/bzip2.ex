defmodule Bzip2 do
  @moduledoc """
  Documentation for Bzip2.
  """

  @on_load {:init, 0}

  def init do
    :ok = :erlang.load_nif('priv/bzip2_nif', 0)
  end

  @doc ~S"""
  Open a bzstream resource, ready for use for compression or decompreession.

      iex> _bz = Bzip2.open()
  """
  def open() do
    raise "NIF open/0 not implemented"
  end

  @doc ~S"""
  Prepares for compression, using default settings for block size, verbosity,
  and work factor.

      iex> bz = Bzip2.open()
      ...> :ok = Bzip2.compressInit(bz)
  """
  def compressInit(_) do
    raise "NIF compressInit/1 not implemented"
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
