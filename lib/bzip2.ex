defmodule Bzip2 do
  @moduledoc """
  Documentation for Bzip2.
  """

  @on_load {:init, 0}

  def init do
    :ok = :erlang.load_nif('priv/bzip2_nif', 0)
  end

  @doc ~S"""
  Open a bz_stream resource, ready for use for compression or decompression.

      iex> bz = Bzip2.open()
      ...> is_reference(bz)
      true
  """
  def open() do
    raise "NIF open/0 not implemented"
  end

  @doc ~S"""
  Prepares for compression, using default settings for block size, verbosity,
  and work factor.

      iex> bz = Bzip2.open()
      ...> Bzip2.compressInit(bz)
      :ok
  """
  def compressInit(_) do
    raise "NIF compressInit/1 not implemented"
  end

  @doc ~S"""
  Prepares for compression, using default settings for block size, verbosity,
  and work factor.

      iex> bz = Bzip2.open()
      ...> Bzip2.compressInit(bz)
      ...> compressed = Bzip2.compress(bz, "hello world")
      ...> IO.inspect(compressed)
  """
  def compress(_, _) do
    raise "NIF compress/2 not implemented"
  end

  @doc ~S"""
  Releases all memory associated with a compression stream.

      iex> bz = Bzip2.open()
      ...> Bzip2.compressInit(bz)
      ...> Bzip2.compressEnd(bz)
      :ok
  """
  def compressEnd(_) do
    raise "NIF compressEnd/1 not implemented"
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
