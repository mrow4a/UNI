# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('parking', '0011_auto_20150716_2039'),
    ]

    operations = [
        migrations.CreateModel(
            name='Lamp_Register_Queue',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('lamp_number', models.CharField(default=b'None', max_length=256, db_column=b'lamp_number', blank=True)),
                ('lamp_request_ip', models.CharField(default=b'None', max_length=256, db_column=b'lamp_ip', blank=True)),
            ],
        ),
    ]
