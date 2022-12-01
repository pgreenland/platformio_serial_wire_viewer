from platformio.public import DeviceMonitorFilterBase

# ITM SWO data format is described in ARMv7-M Architecture Reference Manual, Appendix D "Debug ITM and DWT Packet Protocol"
class ITM_SWO(DeviceMonitorFilterBase):
	NAME = "itm_swo"

	def __init__(self, *args, **kwargs):
		# Construct parent
		super().__init__(*args, **kwargs)

		# Reset current payload length
		self.payload_len = 0

	def rx(self, text):
		"""Process inbound data"""

		# Init output
		output = ""

		# Process input one character at a time
		for c in text:
			# Process character
			if self.payload_len > 0:
				# Payload bytes remain, pass through
				output += c

				# Decrement payload length
				self.payload_len -= 1

			else:
				# No payload remaining, read next header
				c = ord(c)
				self.payload_len = (c & 0x3) >> 0
				self.payload_src = (c & 0x4) >> 3
				self.itm_port = (c & 0xf0) >> 4

				# Sanity check header
				if self.payload_len != 1 or self.payload_src != 0 or self.itm_port != 0:
					# Reset payload, ignore this packet
					self.payload_len = 0

		# Provide processed output
		return output

	def tx(self, text):
		"""Process outbound data"""

		# Do nothing to transmitted data
		return text
