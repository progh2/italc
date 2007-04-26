/*
 * dsa_key.h - easy to use C++ classes for dealing with DSA-keys, -signatures
 *             etc.
 *
 * Copyright (c) 2006 Tobias Doerffel <tobydox/at/users/dot/sf/dot/net>
 *
 * This file is part of iTALC - http://italc.sourceforge.net
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program (see COPYING); if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 */

#ifndef _DSA_KEY_H
#define _DSA_KEY_H

#include <QtCore/QByteArray>
#include <QtCore/QString>

#include <openssl/dsa.h>


class dsaKey
{
public:
	enum keyType
	{
		Public,
		Private
	} ;

	// constructor
	dsaKey( const keyType _type ) :
		m_dsa( NULL ),
		m_type( _type )
	{
	}

	// destructor
	virtual ~dsaKey()
	{
		if( isValid() )
		{
			DSA_free( m_dsa );
		}
	}

	// return key-type
	inline keyType type( void ) const
	{
		return( m_type );
	}

	inline bool isValid( void ) const
	{
		return( m_dsa != NULL );
	}

	virtual void load( const QString & _file,
				QString _passphrase = QString::null ) = 0;
	virtual void save( const QString & _file,
				QString _passphrase = QString::null ) const = 0;

	bool verifySignature( const QByteArray & _data,
					const QByteArray & _signature ) const;

	const DSA * dsaData( void ) const
	{
		return( m_dsa );
	}

	static QByteArray generateChallenge( void );


protected:
	DSA * m_dsa;
	keyType m_type;

} ;




class privateDSAKey : public dsaKey
{
public:
	// constructor - load private key from file
	privateDSAKey( const QString & _file,
				const QString & _passphrase = QString::null ) :
		dsaKey( Private )
	{
		load( _file, _passphrase );
	}

	// constructor - generate new private key with given number of bits
	privateDSAKey( const unsigned int _bits );

	// returns signature for data
	QByteArray sign( const QByteArray & _data ) const;

	virtual void load( const QString & _file,
					QString _passphrase = QString::null );
	virtual void save( const QString & _file,
				QString _passphrase = QString::null ) const;

} ;




class publicDSAKey : public dsaKey
{
public:
	// constructor - load public key from file
	publicDSAKey( const QString & _file ) :
		dsaKey( Public )
	{
		load( _file );
	}

	// constructor - derive public key from private key
	explicit publicDSAKey( const privateDSAKey & _pkey );

	virtual void load( const QString & _file,
					QString _passphrase = QString::null );
	virtual void save( const QString & _file,
				QString _passphrase = QString::null ) const;

} ;


#endif